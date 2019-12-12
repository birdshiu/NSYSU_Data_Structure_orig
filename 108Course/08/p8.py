import tkinter as tk
import tkinter.messagebox
import tkinter.ttk
from tkinter.filedialog import *
from functools import partial
import cppyy

cppyy.include('./huffmanInCpp.hpp')
cppyy.include('./tools.hpp')
cppyy.load_library('huffman.so')


def doDecode(fileName):
    print('start decode ' + fileName)
    isSuccess = cppyy.gbl.decompress(fileName)
    if isSuccess:
        tkinter.messagebox.showinfo(title='Success!', message='done and exit')
    else:
        tkinter.messagebox.showinfo(title='Failed!', message='Failed')
    sys.exit(0)


def doEncode(fileName):
    print('start encode ' + fileName)
    isSuccess = cppyy.gbl.compress(fileName)
    if isSuccess:
        tkinter.messagebox.showinfo(title='Success!', message='done and exit')
    else:
        tkinter.messagebox.showinfo(title='Failed!', message='Failed')
    sys.exit(0)


window = tk.Tk()
window.title('Hello world for Huffman_coding')
window.geometry('500x300')

# setting Label
title = tk.Label(text='Hello world')

# put Lable on to the content
title.pack()

while(True):
    fileName = tk.filedialog.askopenfilename()
    if(fileName):
        inFile = open(fileName, 'r')
        print("Got file: " + fileName)
        break


doDecodeButton = tk.Button(window, text='decode',
                           width=10, height=2, command=partial(doDecode, fileName)).pack()
doEncodeButton = tk.Button(window, text='encode',
                           width=10, height=2, command=partial(doEncode, fileName)).pack()


# loop while you ckick
window.mainloop()
