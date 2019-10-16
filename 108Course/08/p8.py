import tkinter as tk
from tkinter.filedialog import *

# do decode
def doDecode(): 
    print('start decode')

# do encode
def doEncode(): 
    print('start encode')

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
                           width=10, height=2, command=doDecode)
doEncodeButton = tk.Button(window, text='encode',
                           width=10, height=2, command=doEncode)
doDecodeButton.pack()
doEncodeButton.pack()

# loop while you ckick
window.mainloop()
