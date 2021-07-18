# coding: utf-8
'''
simple_params.pklファイルからWeightとBiasを抜き出して保存します。
'''
import pickle
import numpy as np
from pprint import pprint

out_dir = "../01_CNN_WB/"

def dump_matrix(mat, h_max, w_max):
  for h in range(0, h_max):
    print('{:3d}: '.format(h), end=' ')
    for w in range(0, w_max):
      print('{: .10f}'.format(mat[h][w]), end=' ')
    print('...')

  for h in range(mat.shape[0]-h_max, mat.shape[0]):
    print('{:3d}: '.format(h), end=' ')
    for w in range(0, w_max):
      print('{: .10f}'.format(mat[h][w]), end=' ')
    print('...')
  print('')

def dump_array(array, num_max):
  for i in range(0, num_max):
    print('{: .10f}'.format(array[i]), end=' ')
  print('...', end='')
  for i in range(array.shape[0]-num_max, array.shape[0]):
    print('{: .10f}'.format(array[i]), end=' ')
  print('\n')
  
with open('simple_params.pkl', 'rb') as f:
  network = pickle.load(f)

W1 = network['W1'].astype(np.float32)
W2 = network['W2'].astype(np.float32)
W3 = network['W3'].astype(np.float32)
b1 = network['b1'].astype(np.float32)
b2 = network['b2'].astype(np.float32)
b3 = network['b3'].astype(np.float32)

print('W1:', W1.shape)
W1_mat = W1.flatten().reshape(30, 5*5)
dump_matrix(W1_mat, 3, 3)
W1_mat.tofile(out_dir+'W1.bin')
np.savetxt(out_dir+'W1.txt', W1_mat,   delimiter='', fmt ='% .10f ')

print('W2:', W2.shape)
dump_matrix(W2, 3, 3)
W2.tofile(out_dir+'W2.bin')
np.savetxt(out_dir+'W2.txt', W2,   delimiter='', fmt ='% .10f ')

print('W2T:', W2.T.shape)
dump_matrix(W2.T, 3, 3)
W2.T.tofile(out_dir+'W2T.bin')
np.savetxt(out_dir+'W2T.txt', W2.T,   delimiter='', fmt ='% .10f ')

print('W3:', W3.shape)
dump_matrix(W3, 3, 3)
W3.tofile(out_dir+'W3.bin')
np.savetxt(out_dir+'W3.txt', W3,   delimiter='', fmt ='% .10f ')

print('b1:', b1.shape)
dump_array(b1, 3)
b1.tofile(out_dir+'b1.bin')
np.savetxt(out_dir+'b1.txt', b1,   delimiter='', fmt ='% .10f ')

print('b2:', b2.shape)
dump_array(b2, 3)
b2.tofile(out_dir+'b2.bin')
np.savetxt(out_dir+'b2.txt', b2,   delimiter='', fmt ='% .10f ')

print('b3:', b3.shape)
dump_array(b3, 3)
b3.tofile(out_dir+'b3.bin')
np.savetxt(out_dir+'b3.txt', b3,   delimiter='', fmt ='% .10f ')