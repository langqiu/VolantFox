name: musically_model_sex
data:
- dataset: [musically_text_sex_training_set]
  sample: {'1':1,'0':0.2}
  get_conv: get_conv3
- dataset: [musically_text_sex_training_set_from_sexbbs]
  sample: {'1':1}
  get_conv: get_conv3
score:
- dataset: [musically_text_sex_testing_set]
  get_conv: get_conv3
parameters: {'sentence_size':25}
location_vec: en_comments.w2v
epoch_num: 50
learn_dir: ""
execute: fit


