import glob
import cv2	# Tested under version 4.1.2.30
import numpy as np
import matplotlib.pyplot as plt

def eval_transform_values(dir, kind, mask=None, m=None):
	files = glob.glob(dir + '/*.jpg')
	files.sort()

	values = [1] if kind == 'scale' else [0]
	for i in range(len(files) - 1):
		im0 = cv2.cvtColor(cv2.imread(files[i]), cv2.COLOR_BGR2GRAY)
		im1 = cv2.cvtColor(cv2.imread(files[i+1]), cv2.COLOR_BGR2GRAY)
		print(files[i], files[i+1])

		if mask:
			im0 = im0[mask['t']:mask['b'], mask['l']:mask['r']]
			im1 = im1[mask['t']:mask['b'], mask['l']:mask['r']]

		criteria = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 200, 0.001)
		_, m = cv2.findTransformECC(im0, im1, m, cv2.MOTION_AFFINE, criteria, None, 5)

		if kind == 'translate':
			translate = (m[0][2]**2 + m[1][2]**2) ** 0.5
			values.append(values[-1] + translate)
		elif kind == 'scale':
			scale_x = (m[0][0]**2 + m[1][0]**2) ** 0.5
			scale_y = (m[0][1]**2 + m[1][1]**2) ** 0.5
			scale = 0.5 * (scale_x + scale_y)
			values.append(values[-1] * scale)
		elif kind == 'rotate':
			rotate0 = np.arctan2(-m[0][1], m[0][0])
			rotate1 = np.arctan2(m[1][0], m[1][1])
			rotate = 0.5 * (rotate0 + rotate1)
			values.append(values[-1] + rotate)

	return values

fig, axs = plt.subplots(3, 3)

values_gta_translate = eval_transform_values('./gta', 'translate')
axs[0][0].plot(values_gta_translate, 'o-')
axs[0][0].set_title('GTA5 pan')

values_amap_translate = eval_transform_values('./amap_pan/', 'translate', {'l':110, 't':135, 'r':625, 'b':1020})
axs[0][1].plot(values_amap_translate, 'o-')
axs[0][1].set_title('Amap iOS pan')

values_amap_scale = eval_transform_values('./amap_zoom/', 'scale', {'l':110, 't':135, 'r':625, 'b':1020})
axs[0][2].plot(values_amap_scale, 'o-')
axs[0][2].set_title('Amap iOS zoom')

# The algorithm is not intelligent enough, so I have to initialize an estimated translation
m = np.array([[1, 0, 0], [0, 1, -100]], np.float32)
values_baidu_translate = eval_transform_values('./baidu_pan/', 'translate', {'l':110, 't':160, 'r':640, 'b':980}, m)
axs[1][0].plot(values_baidu_translate, 'o-')
axs[1][0].set_title('Baidu map iOS pan')

values_baidu_rotate = eval_transform_values('./baidu_rotate/', 'rotate', {'l':110, 't':70, 'r':640, 'b':1260})
axs[1][1].plot(values_baidu_rotate, 'o-')
axs[1][1].set_title('Baidu map iOS rotate')

values_baidu_scale = eval_transform_values('./baidu_zoom/', 'scale', {'l':110, 't':150, 'r':640, 'b':1150})
axs[1][2].plot(values_baidu_scale, 'o-')
axs[1][2].set_title('Baidu map iOS zoom')

values_apple_scale = eval_transform_values('./apple_map_zoom/', 'scale', {'l':110, 't':166, 'r':640, 'b':1126})
axs[2][0].plot(values_apple_scale, 'o-')
axs[2][0].set_title('Apple map zoom')

values_apple_rotate = eval_transform_values('./apple_map_rotate/', 'rotate', {'l':110, 't':166, 'r':640, 'b':1126})
axs[2][1].plot(values_apple_rotate, 'o-')
axs[2][1].set_title('Apple map rotate')

values_apple_pan = eval_transform_values('./apple_map_pan/', 'translate', {'l':110, 't':166, 'r':640, 'b':1126})
axs[2][2].plot(values_apple_pan, 'o-')
axs[2][2].set_title('Apple map pan')

plt.show()
