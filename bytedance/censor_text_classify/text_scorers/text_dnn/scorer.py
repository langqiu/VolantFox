#coding:utf-8

import sys
import json
from collections import namedtuple
sys.path.insert(0,"/opt/tiger/text_lib/")

#import gpu_mxnet as mx

import mxnet as mx
import numpy as np
from collections import namedtuple

def get_w2v(location):
    w2v = {}
    num = 0
    with open(location) as f:
        for line in f:
            line = line.decode('utf-8')
            xs = line.strip().split()
            if len(xs) == 2:
                continue
            if num == 0:
                num = len(xs)
            if num !=0 and len(xs) != num:
                #print line
                continue
            w2v[xs[0]] = map(float,xs[1:])
    num = len(w2v.itervalues().next())
    return num, w2v


def get_vocab(location):
    id2v = []
    w2id = {}
    idx = 0
    num = 0
    with open(location) as f:
        for line in f:
            line = line.decode('utf8')
            xs = line.strip().split()
            if len(xs) == 2:
                continue
            if num == 0:
                num = len(xs)
            if num !=0 and len(xs) != num:
                #print line
                continue
            w2id[xs[0]] = idx
            idx += 1
            id2v.append(map(float,xs[1:]))
    return np.array(id2v), w2id

def pad_sentences(sentences, padding_word="</s>", sentence_size=25):
    """
    Pads all sentences to the same length. The length is defined by the longest sentence.
    Returns padded sentences.
    """
    #sequence_length = max(len(x) for x in sentences)
    sequence_length = sentence_size
    padded_sentences = []
    for i in range(len(sentences)):
        sentence = sentences[i]
        if len(sentence) < sequence_length:
            num_padding = sequence_length - len(sentence)
            new_sentence = sentence + [padding_word] * num_padding
        else:
            new_sentence = sentence[:sequence_length]
        padded_sentences.append(new_sentence)
    return padded_sentences

DNNModel = namedtuple('DNNModel', ['dnn_exec', 'symbol', 'data', 'label', 'param_blocks'])

def load_text_dnn_symbol(location, batch_size=128, is_train=False):
    with open(location) as f:
        text = f.read()
    x = json.loads(text)
    for n in x['nodes']:
        if n['op'] == 'Dropout' and not is_train:
            n['param']['p'] = "0.0"
    text = json.dumps(x)
    symbol = mx.symbol.load_json(text)
    return symbol



class TextFastDNNScorer(object):
    
    def __init__(self, params):
        batch_size = 1
        

        location_symbol = params['location_symbol']
        location_model = params['location_model']
        location_vec = params['location_vec']
        location_size = params['location_size']
        id2v, self.w2id = get_vocab(location_vec)

        ctx = mx.cpu()
        sm = load_text_dnn_symbol(location_symbol, batch_size, is_train=False)


        with open(location_size) as fin:
            model_size = json.loads(fin.readline())
        sentence_size = model_size['sentence_size']
        

        input_shapes = {'data': (batch_size, sentence_size)}
        arg_shape, out_shape, aux_shape = sm.infer_shape(**input_shapes)
        arg_names = sm.list_arguments()

        p = mx.nd.load(location_model)
        arg_dict = {}
        for k,v in p.items():
            if k.startswith('arg:'):
                k = k[4:]
                arg_dict[k] = v
        arg_arrays = []
        for name, s in zip(arg_names, arg_shape):
            if name in ['softmax_label', 'data']:
                arg_arrays.append(mx.nd.zeros(s, ctx))
            elif name == "embedding_weight":
                arg_arrays.append(mx.nd.array(id2v))
            else:
                arg_arrays.append(arg_dict[name])  


        dnn_exec = sm.bind(ctx=ctx, args=arg_arrays)

        data = dnn_exec.arg_dict['data']
        label = dnn_exec.arg_dict['softmax_label']

        self.scorer = DNNModel(dnn_exec=dnn_exec, symbol=sm, data=data, label=label, param_blocks=None)
        self.sentence_size = sentence_size

    @staticmethod
    def check_parameters(params):
        for n in ['location_symbol', 'location_model', 'location_vec', 'location_size']:
            if n not in params:
                return False
        return True

    def conv(self, sentence): # 分词之后的结果
        if isinstance(sentence, str):
            sentence = sentence.decode('utf8')
        sentences = [sentence]
        sentences = [sentence.lower().split() for sentence in sentences]
        sentences_padded = pad_sentences(sentences,sentence_size=self.sentence_size)
        x_vec = []
        for sent in sentences_padded:
            vec = []
            for word in sent:
                if word in self.w2id:
                    vec.append(self.w2id[word])
                else:
                    vec.append(self.w2id['</s>'])
            x_vec.append(vec)
        x_vec = np.array(x_vec)

        return x_vec

    def get_score(self,sentence):
        x_vec = self.conv(sentence)

        self.scorer.data[:] = x_vec
        self.scorer.dnn_exec.forward(is_train=False)
        res = self.scorer.dnn_exec.outputs[0].asnumpy()
        return float(res[0][1])


def get_text_dnn_symbol(num_hiddens=[100], num_label=2, dropout=0.0):
    data = mx.sym.Variable('data')
    label = mx.sym.Variable('softmax_label')
    layers = [data]
    for i in xrange(len(num_hiddens)):
        z = mx.sym.FullyConnected(data=layers[-1], name='fc%s' % i, num_hidden=num_hiddens[i])
        a = mx.sym.Activation(data=z, name='relu%s' % i, act_type="relu")
        layers.append(a)
    if dropout > 0.0:
        layers.append(mx.sym.Dropout(data=layers[-1], p=dropout))
    output = mx.sym.FullyConnected(data=layers[-1], name='fc', num_hidden=num_label)
    softmax_label = mx.sym.SoftmaxOutput(data=output, label=label, name='softmax')
    return softmax_label

def load_dnn(location_model, location_size):
    # load model size
    with open(location_size) as fin:
        model_size =json.loads(fin.readline())
    ctx = mx.cpu()
    # build model structure
    sm = get_text_dnn_symbol(
        model_size['num_hiddens'], 
        model_size.get('num_label', 2),
        0.0
    )

    input_shapes = {'data': (1, model_size['vec_size'])}
    arg_shape, out_shape, aux_shape = sm.infer_shape(**input_shapes)
    arg_names = sm.list_arguments()

    # load model paramters
    p = mx.nd.load(location_model)
    arg_dict = {}
    for k, v in p.items():
        # print k
        if k.startswith('arg:'):
            k = k[4:]
            arg_dict[k] = v
    arg_arrays = []
    for name, s in zip(arg_names, arg_shape):
        if name in ['softmax_label', 'data']:
            arg_arrays.append(mx.nd.zeros(s, ctx))
        else:
            arg_arrays.append(arg_dict[name])  

    dnn_exec = sm.bind(ctx=ctx, args=arg_arrays)

    data = dnn_exec.arg_dict['data']
    label = dnn_exec.arg_dict['softmax_label']

    return DNNModel(dnn_exec=dnn_exec, symbol=sm, data=data, label=label, param_blocks=None)


class TextDNNScorer(object):
    vectors = {}
    vec_size = {}
    def __init__(self, params):
        batch_size = 1
        location_model = params['location_model']
        location_vec = params['location_vec']
        location_size = params['location_size']
        id2v, self.w2id = get_vocab(location_vec)

        self.scorer = load_dnn(location_model, location_size)

        if location_vec not in TextDNNScorer.vectors:
            TextDNNScorer.vec_size[location_vec], TextDNNScorer.vectors[location_vec] = get_w2v(location_vec)

        self.location_vec = location_vec
        with open(location_size) as fin:
            model_size =json.loads(fin.readline())
        # self.vec_size = model_size['vec_size']
        # assert self.vec_size == TextDNNScorer.vec_size[location_vec]
        self.sentence_size = model_size['sentence_size']
        self.location_vec = location_vec

        # ctx = mx.cpu()
        # sm = load_text_dnn_symbol(location_symbol, batch_size, is_train=False)


        # with open(location_size) as fin:
        #     model_size = json.loads(fin.readline())
        # sentence_size = model_size['sentence_size']
        

        # input_shapes = {'data': (batch_size, sentence_size)}
        # arg_shape, out_shape, aux_shape = sm.infer_shape(**input_shapes)
        # arg_names = sm.list_arguments()

        # p = mx.nd.load(location_model)
        # arg_dict = {}
        # for k,v in p.items():
        #     if k.startswith('arg:'):
        #         k = k[4:]
        #         arg_dict[k] = v
        # arg_arrays = []
        # for name, s in zip(arg_names, arg_shape):
        #     if name in ['softmax_label', 'data']:
        #         arg_arrays.append(mx.nd.zeros(s, ctx))
        #     elif name == "embedding_weight":
        #         arg_arrays.append(mx.nd.array(id2v))
        #     else:
        #         arg_arrays.append(arg_dict[name])  


        # dnn_exec = sm.bind(ctx=ctx, args=arg_arrays)

        # data = dnn_exec.arg_dict['data']
        # label = dnn_exec.arg_dict['softmax_label']

        # self.scorer = DNNModel(dnn_exec=dnn_exec, symbol=sm, data=data, label=label, param_blocks=None)
        # self.sentence_size = sentence_size

    @staticmethod
    def check_parameters(params):
        for n in ['location_model', 'location_vec', 'location_size']:
            if n not in params:
                return False
        return True

    def conv(self, sentence): # 分词之后的结果
        if isinstance(sentence, str):
            sentence = sentence.decode('utf8')
        w2v = TextDNNScorer.vectors[self.location_vec]
        sentences = [sentence]
        sentences = [sentence.lower().split() for sentence in sentences]
        sentences_padded = pad_sentences(sentences, sentence_size=self.sentence_size)
        x_vec = []
        for sent in sentences_padded:
            vec = []
            for word in sent:
                if word in w2v:
                    vec.append(w2v[word])
                else:
                    vec.append(w2v['</s>'])
            x_vec.append(vec)
        x_vec = np.array(x_vec)
        return np.sum(x_vec, axis=1)

    def get_score(self, sentence):
        x_vec = self.conv(sentence)

        self.scorer.data[:] = x_vec
        self.scorer.dnn_exec.forward(is_train=False)
        res = self.scorer.dnn_exec.outputs[0].asnumpy()
        return float(res[0][1])


if __name__ == '__main__':

    print 'x'

    location_model = ''
    location_model = '../model/dnn-0001.params.cpu'
    location_symbol = "../model/dnn-symbol.json"
    location_vec = '../vectors/ugc.vec'
    location_size = "../model/size.txt"
    
    sentence = u'多少 人 是 被 封面 骗 进来 的'
    # vec_size = 80
    sentence_size = 25
    scorer = TextDNNScorer({
        'location_model':location_model, 
        'location_symbol':location_symbol, 
        'location_vec':location_vec,
        'location_size':location_size
    })

    res = scorer.get_score(sentence)
    print res

