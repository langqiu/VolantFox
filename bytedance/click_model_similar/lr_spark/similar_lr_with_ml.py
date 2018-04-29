from pyspark.sql import SparkSession
from pyspark import SparkContext
from pyspark.conf import SparkConf
from pyspark.ml.linalg import Vectors
from pyspark.ml.classification import LogisticRegression
from pyspark.ml.evaluation import BinaryClassificationEvaluator

def getFeature2(line):
    sep = line.split('\t')
    feature_list = sep[2]
    return [(v, 1) for v in set(map(lambda x : int(x.split(':')[0]), feature_list.split(',')))]

def getFeature(line):
    sep = line.split(' ')
    return [(v, 1) for v in set(map(lambda x : int(x.split(':')[0]), sep[1:]))]

def parseData2(line, feature_count):
    sep = line.split('\t')
    log_id = sep[0]
    label = int(sep[1])
    feature_list = sep[2]
    index_list = map(lambda x : int(x.split(':')[0]), feature_list.split(','))
    value_list = map(lambda x : float(x.split(':')[1]), feature_list.split(','))
    return (label, Vectors.sparse(feature_count, index_list, value_list))

def parseData(line, feature_count):
    sep = line.split(' ')
    label = int(sep[0])
    index_list = map(lambda x : int(x.split(':')[0])-1, sep[1:])
    value_list = map(lambda x : float(x.split(':')[1]), sep[1:])
    return (label, Vectors.sparse(feature_count, index_list, value_list))

sc = SparkContext(appName="lr_train")
spark = SparkSession.builder.config(conf=SparkConf()).getOrCreate()
r = sc.textFile('s3://musically-bigdata/recommend_script/data/similar_click_model/similar_click_model_one_hot_feature_label/dt=20170912')
#r = sc.textFile('s3://musically-bigdata/recommend_script/data/similar_click_model/similar_click_model_one_hot_feature_label_libsvm/dt=20170912')
feats = r.flatMap(lambda x : getFeature2(x)).reduceByKey(lambda x,y : 1).collectAsMap()
feat_num = len(feats)
print('feature size is : ' + str(feat_num))

r_process = r.map(lambda x : parseData2(x, feat_num))
r_test, r_train = r_process.randomSplit(weights=[0.2, 0.8], seed=1)
training = spark.createDataFrame(r_train, ["label", "features"])
testing = spark.createDataFrame(r_test, ["label", "features"])

lr = LogisticRegression()
model = lr.fit(training)
#model.write().overwrite().save("s3://musically-bigdata/recommend_script/data/similar_click_model/similar_click_model_one_hot_feature_label/lr_model/dt=20170912")
print("Training AUC = " + str(model.summary.areaUnderROC))
prediction = model.transform(testing)
evaluator = BinaryClassificationEvaluator(labelCol="label", rawPredictionCol="prediction")
print("Testing AUC = " + str(evaluator.evaluate(prediction, {evaluator.metricName: "areaUnderROC"})))
#print "Parameters"
#print model.coefficients

spark.stop()
