# av-soundsquares
put your volume low for your sanity
https://bgreer.co.uk/sound-project

This app expands on techniques I've used in graphics throughout year one and two graphics modules.
I used pixel binning to reduce the webcam footage into a grid of squares, determined on the GUI, which take 
the first pixel of the bin and colour the square to match that. I then draw the squares using mapped 
brightness or saturation values to scale to a maximum of the radius from the GUI. Each box stores the relevant value in a 
2D array, so I can access it later. Then, I use an array of 9 random boxes chosen 
every time the grid is re-initialised, and create a series of sine waves that use the scaling values 
(brightness/saturation) to alter the frequencies. I add these together using ./25 scaler multipliers 
to make them sound semi decent/bearable, before using this as the output. The result is a program that 
reacts to the video in real time, creating sound from video. I think this has pretty good foundings as a 
creative piece, but I think it might be more interesting to use frame differencing at a later date. 
I also would like to try more to force harmonies into the sine waves- I used the scalar maths but that relies on using a base note, 
which I think I didn’t do quite right. I’m still happy with the result though.
