import tkinter as tk
import tkinter.messagebox
import tkinter.ttk
import platform
from tkinter.filedialog import *
from functools import partial
from ctypes import cdll
#from huffmanCoding import *


"""

if platform.system() == 'Windows':
    libc = cdll.LoadLibrary('huffman.dll')
elif platform.system() == 'Linux':
    libc = cdll.LoadLibrary('huffman.so')

class huffman(object):
    def __init__(self):
        self.obj = libc.
"""


def allDone():
    tkinter.messagebox.showinfo(title='Done!', message='done and exit')
    # remember to close file
    sys.exit(0)

# do decode


def doDecode(fileName):
    print('start decode' + fileName)
    h = HuffmanCoding(fileName)
    h.decompress(fileName)
    allDone()


# do encode

def doEncode(fileName):
    print('start encode' + fileName)
    h = HuffmanCoding(fileName)
    h.compress()
    allDone()


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
