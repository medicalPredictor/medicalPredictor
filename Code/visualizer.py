from tkinter import *
from PIL import ImageTk, Image
import matplotlib.pyplot as plt


params = Tk() # The first window

# Makes a new window with an image in the middle
averagedSims = Toplevel()
averagedSims.title("Averaged Simulations")
averagedSims.geometry("1000x800")
avgImage = Frame(averagedSims,width = 700, height = 600)
avgImage.pack() # 
avgImage.place(anchor = 'center', relx=0.5, rely = 0.5)

# Pull from file
data = open("DATA.DAT",'r') # check data example file to make sure, should be I D R then varients
contents = data.read()
# Break into experiments
# Break into individual stuffs  
print(contents)

# testData = []
# [testData for x in range(100)]
# plt.scatter(testData,testData,'ro')
# plt.savefig("testData.png")

# Actually draws the graph
avgGraph = ImageTk.PhotoImage(Image.open("testData.png"))
avgLabel = Label(avgImage, image = avgGraph)
avgLabel.pack()




mainloop()


