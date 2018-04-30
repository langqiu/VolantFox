from pyspark.sql import SparkSession
from pyspark import SparkContext
from pyspark.conf import SparkConf
from pyspark.ml.linalg import Vectors
from pyspark.ml.classification import LogisticRegression
from pyspark.ml.evaluation import BinaryClassificationEvaluator

spark = SparkSession\
        .builder\
        .appName("lr_train")\
        .getOrCreate()
r = spark.read.format("libsvm")\
        .option("numFeatures", "655744")\
        .load('s3://musically-bigdata/recommend_script/data/similar_click_model/similar_click_model_one_hot_feature_label_libsvm/dt=20170912/')
r_test, r_train = r.randomSplit(weights=[0.2, 0.8], seed=1234)

#lr = LogisticRegression(regParam=0.01, elasticNetParam=0.1)
lr = LogisticRegression()
model = lr.fit(r_train)
print("Training AUC = " + str(model.summary.areaUnderROC))
#model.write().overwrite().save("s3://musically-bigdata/recommend_script/data/similar_click_model/similar_click_model_one_hot_feature_label/lr_model/dt=20170912")
evaluator = BinaryClassificationEvaluator(labelCol="label", rawPredictionCol="prediction")
prediction = model.transform(r_test)
print("Testing AUC = " + str(evaluator.evaluate(prediction, {evaluator.metricName: "areaUnderROC"})))
#print "Parameters"
#print model.coefficients

spark.stop()

#option("numFeatures", "764696")\
