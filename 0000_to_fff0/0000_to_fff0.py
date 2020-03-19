from PIL import Image

for filename in ['example.png']:
    im = Image.open(filename).convert('RGBA')
    data = im.getdata()

    newdata = list()
    for px in data:
        if px[0] == 0 and px[1] == 0 and px[2] == 0 and px[3] == 0:
            newdata.append((255, 255, 255, 0))
        else:
            newdata.append(px)

    im.putdata(newdata)
    im.save('fff0_' + filename, "PNG")