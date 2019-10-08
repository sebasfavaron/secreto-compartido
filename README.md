Implementation of [Verifiable Image Secret Sharing Using Matrix Projection](https://www.sciencedirect.com/science/article/pii/S1110866514000334)

Authors: Joaquin Ormachea, Sebastian Favaron, Diego Orlando

## Compile and setup
In the projects root folder open a terminal and run

```chmod +x run.sh ; ./run.sh ; cd build/src/integration_module```

Then:

```./cripto_secreto_compartido OPTIONS```

## Extract from the man page that shows how to use it (replace imhide with ./cripto_secreto_compartido or create an alias)

```
NAME
    imhide - hide and distribute/retrieve image in/from other images

SYNOPSIS
	imhide -d [OPTION...]
	imhide -r [OPTION...]

DESCRIPTION
	imhide distributes (and later retrieves) an 8bit bmp image into n 24bit bmp images, from which k (smaller
	than n) are necessary to retrieve the secret image

OPTIONS
	Generic Program Information
		--help Output a usage message and exit.

	Image encription mode
		-d: image distribution (encription) mode
		OR
		-r: image retrieval (decription) mode
	
	Additional options
		-s "secretImage": image to hide (in case -d was used) or filename of the retrieved image (in case of -r)
		   (must be 8bit bmp image)
		-m "watermark": b&w image to serve as watermark to verify the process (in case -d was used) or the 
		   transformation of this watermark (in case of -r)(must be 8bit bmp image and same size as the secret image)
		-k number: number of the minimum shadows necessary to retrieve the secret in a (k, n) scheme
		-n number: number of the total shadows in which to distribute the secret in a (k, n) scheme
		-dir directory: directory of the images that will distribute the secret (in case of -d) or directory of
		     already modified images (in case of -r)(must contain 24bit bmp images)
```
