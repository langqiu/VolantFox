#coding:utf-8
import os
import sys
import json
from collections import namedtuple
sys.path.insert(0,"/opt/tiger/text_lib/")
# sys.path.insert(0, '/opt/tiger/mxnet/python')

if os.getenv('MXNET_TYPE') == 'gpu_mxnet':
    import gpu_mxnet as mx
else:
    import mxnet as mx


# import mxnet as mx
import numpy as np

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

def pad_sentences(sentences, padding_word="</s>",sentence_size=25):
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


CNNModel = namedtuple('CNNModel', ['cnn_exec', 'symbol', 'data', 'label', 'param_blocks'])
def load_text_cnn_symbol(location, batch_size=128, is_train=False):
    with open(location) as f:
        text = f.read()
    x = json.loads(text)
    # 改变预测时的batch_size
    for n in x['nodes']:
        if n['op'] == 'Reshape':
            shape = eval(n['param']['target_shape'])
            shape = (batch_size, ) + shape[1:]
            n['param']['target_shape'] = '(%s)' % ','.join([str(s) for s in shape])
        if n['op'] == 'Dropout' and not is_train:
            n['param']['p'] =  "0.0"

    text = json.dumps(x)
    sm = mx.symbol.load_json(text)
    return sm

def get_text_cnn_symbol(sentence_size, vec_size, batch_size, num_label, filter_list, num_filter, dropout):
    input_x = mx.sym.Variable('data')  # [batch_size, 1, sentence_size, dim]
    input_y = mx.sym.Variable('softmax_label')

    pooled_outputs = []
    for i, filter_size in enumerate(filter_list):
        convi = mx.sym.Convolution(data=input_x, kernel=(filter_size, vec_size), num_filter=num_filter, name=('convolution%s' %i))
        relui = mx.sym.Activation(data=convi, act_type='relu', name=('activation%s' % i))
        pooli = mx.sym.Pooling(data=relui, pool_type='max', kernel=(sentence_size - filter_size + 1, 1), stride=(1,1), name=('pooling%s' % i))
        pooled_outputs.append(pooli)

    total_filters = num_filter * len(filter_list)
    concat = mx.sym.Concat(dim=1, *pooled_outputs)
    h_pool = mx.sym.Reshape(data=concat, target_shape=(batch_size, total_filters))
    if dropout > 0.0:
        h_drop = mx.sym.Dropout(data=h_pool, p=dropout)
    else:
        h_drop = h_pool
    cls_weight = mx.sym.Variable('cls_weight')
    cls_bias = mx.sym.Variable('cls_bias')
    fc = mx.sym.FullyConnected(data=h_drop, weight=cls_weight, bias=cls_bias, num_hidden=num_label)
    sm = mx.sym.SoftmaxOutput(data=fc, label=input_y, name='softmax')
    return sm

def load_cnn(location_model, location_size, batch_size=1, gpu=None):

    # load model size
    with open(location_size) as fin:
        model_size =json.loads(fin.readline())
    if gpu != None:
        print "Use gpu(%s)" % gpu
        ctx = mx.gpu(gpu)
    else:
        print "Use cpu"
        ctx = mx.cpu()
    # build model structure
    sm = get_text_cnn_symbol(
        model_size['sentence_size'],
        model_size['vec_size'],
        batch_size,
        model_size.get('num_label', 2),
        model_size.get('filter_list', [1,2,3,4]),
        model_size.get('num_filter', 60),
        0.0
    )

    input_shapes = {'data': (batch_size, 1, model_size['sentence_size'], model_size['vec_size'])}
    arg_shape, out_shape, aux_shape = sm.infer_shape(**input_shapes)
    arg_names = sm.list_arguments()

    # load model paramters
    p = mx.nd.load(location_model)
    arg_dict = {}
    for k, v in p.items():
        # print k
        if k.startswith('arg:'):
            k = k[4:]
            arg_dict[k] = mx.nd.array(v.asnumpy(), ctx)
    arg_arrays = []
    for name, s in zip(arg_names, arg_shape):
        if name in ['softmax_label', 'data']:
            arg_arrays.append(mx.nd.zeros(s, ctx))
        else:
            arg_arrays.append(arg_dict[name])

    cnn_exec = sm.bind(ctx=ctx, args=arg_arrays)

    data = cnn_exec.arg_dict['data']
    label = cnn_exec.arg_dict['softmax_label']

    return CNNModel(cnn_exec=cnn_exec, symbol=sm, data=data, label=label, param_blocks=None)

def load_embedding_cnn(id2v, location_symbol, location_model, location_size):
    ctx = mx.cpu()
    # load model size
    with open(location_size) as fin:
        model_size =json.loads(fin.readline())
    sm = load_text_cnn_symbol(location_symbol, batch_size=1, is_train=False)
    input_shapes = {'data': (1, model_size['sentence_size'])}
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
        elif name == "embedding_weight":
            arg_arrays.append(mx.nd.array(id2v, ctx))
        else:
            arg_arrays.append(arg_dict[name])

    cnn_exec = sm.bind(ctx=ctx, args=arg_arrays)

    data = cnn_exec.arg_dict['data']
    label = cnn_exec.arg_dict['softmax_label']

    return CNNModel(cnn_exec=cnn_exec, symbol=sm, data=data, label=label, param_blocks=None)

class TextFastCNNScorer(object):
    def __init__(self, params):
        batch_size = 1
        model_size = None
        location_symbol = params['location_symbol']
        location_model = params['location_model']
        location_vec = params['location_vec']
        location_size = params['location_size']
        id2v, self.w2id = get_vocab(location_vec)

        self.scorer = load_embedding_cnn(id2v, location_symbol, location_model, location_size)
        # load model size
        with open(location_size) as fin:
            model_size =json.loads(fin.readline())
        self.vec_size = model_size['vec_size']
        self.sentence_size = model_size['sentence_size']

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

    def get_score(self, sentence):
        x_vec = self.conv(sentence)
        self.scorer.data[:] = x_vec
        self.scorer.cnn_exec.forward(is_train=False)
        res = self.scorer.cnn_exec.outputs[0].asnumpy()
        return float(res[0][1])


class TextCNNScorer(object):
    vectors = {}
    vec_size = {}
    def __init__(self, params):
        model_size = None
        location_model = params['location_model']
        location_vec = params['location_vec']
        location_size = params['location_size']
        self.score_threshold = params.get('score_threshold', 0.5)
        gpu = params.get('gpu')
        self.batch_size = params.get('batch_num', 1)
        
        self.scorer = load_cnn(location_model, location_size, self.batch_size, gpu)

        if location_vec not in TextCNNScorer.vectors:
            TextCNNScorer.vec_size[location_vec], TextCNNScorer.vectors[location_vec] = get_w2v(location_vec)

        self.location_vec = location_vec


        with open(location_size) as fin:
            model_size =json.loads(fin.readline())
        self.vec_size = model_size['vec_size']
        assert self.vec_size == TextCNNScorer.vec_size[location_vec]
        self.sentence_size = model_size['sentence_size']
        self.location_vec = location_vec

    @staticmethod
    def check_parameters(params):
        for n in ['location_model', 'location_vec', 'location_size']:
            if n not in params:
                return False
        return True

    def conv(self, sentences):
        new_sents = []
        for sentence in sentences:
            if isinstance(sentence, str):
                sentence = sentence.decode('utf8')

            new_sents.append(sentence.lower().split())
        w2v = TextCNNScorer.vectors[self.location_vec]
        #sentences = [sentence.lower().split() for sentence in sentences]
        sentences_padded = pad_sentences(new_sents, sentence_size=self.sentence_size)
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
        #x_vec = np.reshape(x_vec, [1, 1, self.sentence_size, self.vec_size])
        return x_vec

    def get_score(self, sentence):
        x_vec = self.conv([sentence])
        x_vec = np.reshape(x_vec, [1, 1, self.sentence_size, self.vec_size])
        self.scorer.data[:] = x_vec
        self.scorer.cnn_exec.forward(is_train=False)
        res = self.scorer.cnn_exec.outputs[0].asnumpy()
        return float(res[0][1])

    def get_scores(self, sentences):
        res = []
        original_num = len(sentences)
        if (len(sentences) % self.batch_size) > 0:
            sentences += [''] * (self.batch_size - (original_num % self.batch_size))

        num_batch = len(sentences) / self.batch_size
        if num_batch == 1:
            x_vec = self.conv(sentences)
            x_vec = np.reshape(x_vec, [self.batch_size, 1, self.sentence_size, self.vec_size])
            self.scorer.data[:] = x_vec
            self.scorer.cnn_exec.forward(is_train=False)
            res.extend(self.scorer.cnn_exec.outputs[0].asnumpy())
        else:
            for i in range(num_batch):
                x_vec = self.conv(sentences[i*self.batch_size: (i+1)*self.batch_size])
                x_vec = np.reshape(x_vec, [self.batch_size, 1, self.sentence_size, self.vec_size])
                self.scorer.data[:] = x_vec
                self.scorer.cnn_exec.forward(is_train=False)
                res.extend(self.scorer.cnn_exec.outputs[0].asnumpy())

        return [float(r[1]) for r in res[:original_num]]

    def _process_batch(self, sentences):
        #nsent = len(sentences)
        rst = []
        if not sentences:
            return rst
        k = 0
        while k*self.batch_size < len(sentences):
            #return rst
            nsent = sentences[k*self.batch_size:(k+1)*self.batch_size]
            rt_num = len(nsent)
            if rt_num < self.batch_size:
                nsent += [""] * (self.batch_size - rt_num)
                res = self.get_scores(nsent)
            rst.extend(res[:rt_num])
            k+=1
        return rst



SimpleScorer = namedtuple('SimpleScorer', ['model', 'vec_type', 'batch_size', 'sentence_len', 'score_threshold'])
class CNNScorerManager():
    scorers = {}
    vectors = {}
    # vec_size = {}
    sent_len = set()
    @classmethod
    def add_scorers(cls, scorer_name, params):
        if scorer_name in cls.scorers:
            logging.info('%s exists!' % scorer_name)
            return

        # 解析参数
        location_model = params['location_model']
        location_vec = params['location_vec']
        location_size = params['location_size']
        score_threshold = params.get('score_threshold', 0.5)
        gpu = params.get('gpu')
        batch_size = params.get('batch_num', 1)
        
        with open(location_size) as fin:
            model_size =json.loads(fin.readline())

        sentence_len = model_size['sentence_size']
        model = load_cnn(location_model, location_size, batch_size, gpu)
        scorer = SimpleScorer(
            model=model, 
            vec_type=location_vec,
            batch_size=batch_size, 
            sentence_len=sentence_len,
            score_threshold=score_threshold
        )
        cls.sent_len.add(sentence_len)
        cls.scorers[scorer_name] = scorer
       
        if location_vec not in cls.vectors:
            _, cls.vectors[location_vec] = get_w2v(location_vec)

    # only supports same batch_size
    @classmethod
    def conv(cls, sentences):
        for i in range(len(sentences)):
            if isinstance(sentences[i], str):
                sentences[i] = sentences[i].decode('utf8')
            sentences[i] = sentences[i].lower().split()

        cov_sentences = {}
        for s in cls.scorers.itervalues():
            # assert s.batch_size == len(sentences)
            if (s.sentence_len, s.vec_type) in cov_sentences:
                continue

            padded = pad_sentences(sentences, sentence_size=s.sentence_len)
            w2v = cls.vectors[s.vec_type]
            x_vec = []
            for sent in padded:
                vec = []
                for word in sent:
                    if word in w2v:
                        vec.append(w2v[word])
                    else:
                        vec.append(w2v['</s>'])
                x_vec.append(vec)
            x_vec = np.array(x_vec)
            vec_size = x_vec.shape[-1]
            x_vec = np.reshape(x_vec, [-1, 1, s.sentence_len, vec_size])
            cov_sentences[(s.sentence_len, s.vec_type)] = x_vec
        return cov_sentences

    @classmethod
    def get_scores(cls, texts):
        if not isinstance(texts, list):
            texts = [texts]
        conv_texts = cls.conv(texts)
        results = {}
        for cat, s in cls.scorers.iteritems():
            s.model.data[:] = conv_texts[(s.sentence_len, s.vec_type)]
            s.model.cnn_exec.forward(is_train=False)
            res = s.model.cnn_exec.outputs[0].asnumpy()
            if float(res[0][1]) > s.score_threshold:
                results[cat] = float(res[0][1])
        return results

    @staticmethod
    def check_parameters(params):
        for n in ['location_model', 'location_vec', 'location_size']:
            if n not in params:
                return False
        return True

#     @staticmethod
#     def get_scores(cls, sentence):
#         for vec_name in cls.vectors.keys():

#         for label in cls.scorers.keys():


if __name__ == '__main__':

    print 'x'

    location_model = ''
    location_model = '../model/cnn-0001.params.cpu'
    location_symbol = "../model/cnn-symbol.json"
    location_vec = '../vectors/ugc.vec'
    location_size = "../model/size.txt"


    sentence = u'收藏 各个 颜色 的 琥珀 分别 有 什么 功效 ？ 琥珀 对 人体 的 保养 有 非常 好 的 作用 ， 不同 琥珀'
    scorer1 = TextCNNScorer({
        'location_model':location_model,
        'location_symbol':location_symbol,
        'location_vec':location_vec,
        'location_size':location_size
        })


    res1 = scorer1.get_score(sentence)
    print res1


    scorer2 = TextFastCNNScorer({
        'location_model':location_model,
        'location_symbol':location_symbol,
        'location_vec':location_vec,
        'location_size':location_size
        })


    res2 = scorer2.get_score(sentence)
    print res2


