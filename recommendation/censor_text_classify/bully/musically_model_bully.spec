name: musically_model_bully
data:
- dataset: [musically_text_bully_training_set]
  sample: {'1':1,'0':0.2}
  get_conv: get_conv3
score:
- dataset: [musically_text_bully_testing_set]
  get_conv: get_conv3
parameters: {'sentence_size':25}
location_vec: en_comments.w2v
epoch_num: 50
learn_dir: ""
execute: fit
