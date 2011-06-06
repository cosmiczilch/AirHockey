#include "ctexture.h"

int CTexture::makeTexture( string Path, int Format ){ 
	path=Path; 
	format = Format; 
	SDL_Surface *temp_image;

	if ( (temp_image = IMG_Load( path.c_str( ) )) ) { 

		// Check that the image's width is a power of 2
		if ( (temp_image->w & (temp_image->w - 1)) != 0 ) {
			printf("warning: %s's width is not a power of 2\n", path.c_str( ) );
		}

		// Also check if the height is a power of 2
		if ( (temp_image->h & (temp_image->h - 1)) != 0 ) {
			printf("warning: %s's height is not a power of 2\n", path.c_str( ) );
		}

		// get the number of channels in the SDL surface
		nOfColors = temp_image->format->BytesPerPixel;
		if (nOfColors == 4)     // contains an alpha channel
		{
			if (temp_image->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
			else
				texture_format = GL_BGRA;
		} else if (nOfColors == 3)     // no alpha channel
		{
			if (temp_image->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_BGR;
		} else {
			printf("warning: the image is not truecolor..  this will probably break\n");
			// this error should not go unhandled
			SDL_Quit();
			exit(1);
		}

		// Have OpenGL generate a texture object handle for us
		glGenTextures( 1, &texture );

		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, texture );

		// Set the texture's stretching properties
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, temp_image->w, temp_image->h, 0,
				texture_format, GL_UNSIGNED_BYTE, temp_image->pixels );
	} 
	else {
		printf("SDL could not load %s : %s\n", path.c_str( ) , SDL_GetError());
		SDL_Quit();
		return 0;
	}    

	// Free the SDL_Surface only if it was successfully created
	if ( temp_image ) { 
		SDL_FreeSurface( temp_image );
	}

	glDisable( GL_TEXTURE_2D );

	return 1; 
}

int CTexture::bindTexture( ){ 
	glEnable( GL_TEXTURE_2D );

	glColor3f( 1.0, 1.0, 1.0 ); 
	glBindTexture( GL_TEXTURE_2D, texture );

	return 1;
}

void CTexture::unbindTexture( ){ 
	glDisable( GL_TEXTURE_2D );

	return; 
}

