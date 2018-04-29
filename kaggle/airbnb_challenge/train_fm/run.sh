#!/usr/bin/env bash

sh train_fm.sh 1
#sh predict.sh 1 training_set 1_train
#sh auc.sh 1 1_train
sh predict.sh 1 testing_set 1_test
sh auc.sh 1 1_test
