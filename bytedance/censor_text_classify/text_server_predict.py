# encoding=utf8
import sys
import os
import re
import json
import logging
import text_scorers

def text_process(text):
    text = ' '+text.lower()+' '
    text = text.replace('\\n', '')
    pattern1 = re.compile(r" [^a-z, ]+")
    pattern2 = re.compile(r"[^a-z, ]+ ")
    list1 = pattern1.findall(text)
    list2 = pattern2.findall(text)
    for c in list1:
        text = text.replace(c,' ')
    for c in list2:
        text = text.replace(c,' ')
    return text

def text_predict(scorer, text):
    return scorer.get_score(text_process(text))

def init_scorer(params, model_type='cnn'):
    if get_scorer(model_type).check_parameters(params):
        return get_scorer(model_type)(params)
    else:
        logging.error("parameters of %s are wrong: %s" % (model_type, json.dumps(params)))
        return None

def get_scorer(model_type):
    if model_type == 'cnn':
        # from text_cnn.scorer import TextCNNScorer
        from text_scorers import TextCNNScorer
        return TextCNNScorer
    elif model_type == 'dnn':
        # from text_dnn.scorer import TextDNNScorer
        from text_scorers import TextDNNScorer
        return TextDNNScorer
    elif model_type == 'fasttext':
        # from text_fasttext.scorer import TextFasttextScorer
        from text_scorers import TextFasttextScorer
        return TextFasttextScorer
    else:
        return None

def test():
    base_path = os.path.dirname(os.path.realpath(__file__))
    model_name = sys.argv[1]
    params = {}
    params['location_size'] = os.path.join(base_path, model_name, 'size.txt')
    params['location_model'] = os.path.join(base_path, model_name, 'params.cpu')
    params['location_vec'] = os.path.join(base_path, 'w2v_dict/en_comments.w2v')

    scorer = init_scorer(params)
    for text in sys.stdin:
        text = text.strip()
        print text + '\t' + str(text_predict(scorer, text))

if __name__ == '__main__':
    test()
