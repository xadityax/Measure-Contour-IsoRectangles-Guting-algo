import matplotlib
import matplotlib.pyplot as plt

#Parsing Coordinates
list_beg=[]
x_coords=[]
y_coords=[]
list_end=[]
with open("inp.txt", "r") as a_file:
    for line in a_file:
        single_line=line.strip()
        beg_ele=[]
        end_ele=[]
        counter=0
        store=""
        char_count=0
        for c in single_line:
            char_count+=1
            if (char_count==len(single_line)):
                counter=0
                store=store+c
                end_ele.append(float(store))
            if (c != ' ') :
                store=store+c
            elif (c==' '):
                counter=counter+1
                if counter==1 or counter==2:
                    beg_ele.append(float(store))
                    store=""
                elif (counter==3) or (counter==4):
                    end_ele.append(float(store))
                    store=""
                    if counter==4:
                        counter=0
        list_beg.append(beg_ele)
        list_end.append(end_ele)

for i in range(len(list_beg)):
    x_coords.append(list_beg[i][0])
    x_coords.append(list_end[i][0])
    y_coords.append(list_beg[i][1])
    y_coords.append(list_end[i][1])

x_min=min(x_coords)
x_max=max(x_coords)
y_min=min(y_coords)
y_max=max(y_coords)

#Parse the horz segs
horz=[]
with open("out_horz.txt", "r") as h_file:
    for line in h_file:
        single_line=line.strip()
        counter=0
        store=""
        char_count=0
        ex1=None
        ex2=None
        why1=None
        for c in single_line:
            seg=[]
            char_count+=1
            if (char_count==len(single_line)):
                counter=0
                store=store+c
                seg.append(ex1)
                seg.append(float(store))
                seg.append(ex2)
                seg.append(float(store))
                horz.append(seg)
            if (c != ' ') :
                store=store+c
            elif (c==' '):
                counter=counter+1
                if counter==1:
                    ex1=float(store)
                    store=""
                elif counter==2:
                    ex2=float(store)
                    store=""
                elif counter==3:
                    why1=float(store)
                    store=""
                    counter=0
            
vert=[]
with open("out_vert.txt", "r") as v_file:
    for line in v_file:
        single_line=line.strip()
        counter=0
        store=""
        char_count=0
        why1=None
        why2=None
        ex1=None
        for c in single_line:
            seg=[]
            char_count+=1
            if (char_count==len(single_line)):
                counter=0
                store=store+c
                seg.append(float(store))
                seg.append(why1)
                seg.append(float(store))
                seg.append(why2)
                vert.append(seg)
            if (c != ' ') :
                store=store+c
            elif (c==' '):
                counter=counter+1
                if counter==1:
                    why1=float(store)
                    store=""
                elif counter==2:
                    why2=float(store)
                    store=""
                elif counter==3:
                    ex1=float(store)
                    store=""
                    counter=0
#Parsing done

fig = plt.figure()
ax = fig.add_subplot(111)

for i in range(len(horz)):
    x_values=[horz[i][0], horz[i][2]]
    y_values=[horz[i][1], horz[i][3]]
    plt.plot(x_values, y_values, color='green')

for i in range(len(vert)):
    x_values=[vert[i][0], vert[i][2]]
    y_values=[vert[i][1], vert[i][3]]
    plt.plot(x_values, y_values, color='green')

for i in range(len(list_beg)):
    rect_plot= matplotlib.patches.Rectangle((list_beg[i][0], list_beg[i][1]),
                                            (list_end[i][0]-list_beg[i][0]), (list_end[i][1]-list_beg[i][1]),
                                            color ='grey',
                                            alpha=1)
    ax.add_patch(rect_plot)

plt.xlim([x_min-1.0, x_max+1.0])
plt.ylim([y_min-1.0, y_max+1.0])
  
plt.show()
