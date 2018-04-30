from pyspark.sql import SparkSession
from pyspark import SparkContext
from pyspark.conf import SparkConf
from pyspark.mllib.classification import LogisticRegressionWithLBFGS
from pyspark.mllib.classification import LogisticRegressionWithSGD
from pyspark.mllib.regression import LabeledPoint
from pyspark.mllib.linalg import Vectors
from pyspark.mllib.evaluation import BinaryClassificationMetrics

def getFeature(line):
    sep = line.split('\t')
    feature_list = sep[2]
    return [(v, 1) for v in set(map(lambda x : int(x.split(':')[0]), feature_list.split(',')))]

def parseData(line, feature_count):
    sep = line.split('\t')
    log_id = sep[0]
    label = int(sep[1])
    feature_list = sep[2]
    index_list = map(lambda x : int(x.split(':')[0]), feature_list.split(','))
    value_list = map(lambda x : float(x.split(':')[1]), feature_list.split(','))
    return LabeledPoint(label, Vectors.sparse(feature_count, index_list, value_list))

def calMetrics(sc, model, r, step):
    metrics = []
    predict_label = r.map(lambda p : (model.predict(p.features), p.label)).collect()
    predict_label = [(float(i), j) for (i,j) in predict_label]
    metrics_sc = sc.parallelize(predict_label)
    metrics_obj = BinaryClassificationMetrics(metrics_sc)
    metrics.append((model.__class__.__name__, metrics_obj.areaUnderROC, metrics_obj.areaUnderPR))
    print step
    print metrics

def calError(model, r, step):
    label_predict = r.map(lambda p : (p.label, model.predict(p.features)))
    train_error = label_predict.filter(lambda lp: lp[0] != lp[1]).count() / float(r.count())
    print(step + " Error = " + str(train_error))
    positive_error = label_predict.filter(lambda lp: lp[0] != lp[1] and lp[0] == 1).count() / float(label_predict.filter(lambda lp: lp[0] == 1).count())
    print("Positive Error = " + str(positive_error))
    negative_error = label_predict.filter(lambda lp: lp[0] != lp[1] and lp[0] == 0).count() / float(label_predict.filter(lambda lp: lp[0] == 0).count())
    print("Negative Error = " + str(negative_error))
    #positive_negative =  float(label_predict.filter(lambda lp: lp[0] == 1).count()) / float(label_predict.filter(lambda lp: lp[0] == 0).count())
    #print("P_N Error = " + str(positive_negative))

################### initialize
sc = SparkContext(appName="lr_lbfgs_train")
spark = SparkSession.builder.config(conf=SparkConf()).getOrCreate()
r = sc.textFile('s3://musically-bigdata/recommend_script/data/similar_click_model/similar_click_model_one_hot_feature_label/dt=20170912')
feats = r.flatMap(lambda x : getFeature(x)).reduceByKey(lambda x,y : 1).collectAsMap();
feat_nums = len(feats)
print("feature size is : " + str(feat_nums))
r_process = r.map(lambda x : parseData(x, feat_nums))
r_train, r_test = r_process.randomSplit(weights=[0.8, 0.2], seed=1234)

################### train
#model = LogisticRegressionWithLBFGS.train(r_train, iterations=100, regParam=1.0, regType='l2')
model = LogisticRegressionWithLBFGS.train(r_train)
#model.save(sc, 's3://musically-bigdata/recommend_script/data/similar_click_model/similar_click_model_one_hot_feature_label/lbfgs_model_001_1_5_oldfeature/')

#calError(model, r_train, "Training")
#calError(model, r_test, "Testing")

################### AUC
calMetrics(sc, model, r_train, "train AUC")
calMetrics(sc, model, r_test, "test AUC")

