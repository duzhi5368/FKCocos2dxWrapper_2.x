//--------------------------------------------------------
#include "../Include/FKCW_RenderEx_BlurTextureFilter.h"
//--------------------------------------------------------
void FKCW_RenderEx_BlurTextureFilter::blurInput(void* input, void* output, CCTexture2DPixelFormat format, int width, int height, CCSize contentSize, int radius, CCRect rect) 
{
	int read, i, xl, yl, yi, ym, ri, riw;
	const int wh = width*height;

	CCPoint size = ccp(rect.size.width, rect.size.height);
	size.x = (size.x==0) ? contentSize.width : size.x;
	size.y = (size.y==0) ? contentSize.height : size.y;

	//Check data
	CCPoint position = ccp(MAX(0, rect.origin.x), MAX(0, rect.origin.y));
	size.x = position.x+size.x-MAX(0, (position.x+size.x)-width);
	size.y = position.y+size.y-MAX(0, (position.y+size.y)-height);

	yi = static_cast<int>(position.y*width);

	//Generate Gaussian kernel
	radius = MIN(MAX(1,radius), 248);
	int kernelSize = 1+radius*2;
	int* kernel = new int[kernelSize];
	int g = 0, sum = 0;

	//Gaussian filter
	for (i = 0; i<radius;i++)
	{
		g = i*i*i+1;
		kernel[i] = kernel[kernelSize-i-1] = g;
		sum+=g*2;
	}

	g = radius*radius;
	kernel[radius] = g;
	sum+=g;

	if(format == kCCTexture2DPixelFormat_RGBA8888) 
	{

		int cr, cg, cb, ca;
		//const
		ccColor4B *originalData = (ccColor4B*)input;
		ccColor4B *data = (ccColor4B*)output;
		ccColor4B *temp = new ccColor4B[wh*4];//malloc(wh*4);
		ccColor4B *pixel;

		//Horizontal blur

		yl = static_cast<int>(position.y);
		int nSizeY = static_cast<int>(size.y);

		for (; yl<nSizeY; yl++) 
		{

			xl = static_cast<int>(position.x);
			int nSizeX = static_cast<int>(size.x);

			for (; xl<nSizeX; xl++) 
			{
				cb = cg = cr = ca = 0;
				ri = xl-radius;
				for (i = 0; i<kernelSize; i++)
				{
					read = yi + ri + i;
					pixel = &originalData[read];
					cr+= pixel->r*kernel[i];
					cg+= pixel->g*kernel[i];
					cb+= pixel->b*kernel[i];
					ca+= pixel->a*kernel[i];
				}
				pixel = &temp[yi+xl];
				pixel->r = cr/sum;
				pixel->g = cg/sum;
				pixel->b = cb/sum;
				pixel->a = ca/sum;
			}
			yi+=width;
		}

		yi = static_cast<int>((position.y)*width);

		//Vertical blur
		yl = static_cast<int>(position.y);
		nSizeY = static_cast<int>(size.y);
		for (; yl<nSizeY; yl++)
		{
			xl = static_cast<int>(position.x);
			int nSizeX = static_cast<int>(size.x);

			for (; xl<nSizeX; xl++)
			{
				cb = cg = cr = ca = 0;
				ri = yl-radius;
				for (i = 0; i<kernelSize; i++)
				{
					if((ri + i) >=position.y && (ri + i) < size.y)
					{
						read = (ri + i) * width + xl;
					}
					else
					{
						read = yl * width + xl;
					}

					pixel = &temp[read];
					cr+= pixel->r * kernel[i];
					cg+= pixel->g * kernel[i];
					cb+= pixel->b * kernel[i];
					ca+= pixel->a * kernel[i];
				}

				pixel = &data[yi+xl];
				pixel->r = cr/sum;
				pixel->g = cg/sum;
				pixel->b = cb/sum;
				pixel->a = ca/sum;
			}

			yi+=width;
		}
		//Free temp data
		free(temp);
	}
	else  if(format == kTexture2DPixelFormat_A8) 
	{
		int ca;
		const unsigned char *originalData = (const unsigned char*) input;
		unsigned char *data = (unsigned char*) output;
		unsigned char *temp = new unsigned char [wh];//malloc(wh);

		//Horizontal blur
		yl = static_cast<int>(position.y);
		int nSizeY = static_cast<int>(size.y);
		for (; yl<nSizeY; yl++)
		{
			xl = static_cast<int>(position.x);
			int nSizeX = static_cast<int>(size.x);

			for (; xl<nSizeX; xl++)
			{
				ca = 0;
				ri = xl-radius;
				for (i = 0; i<kernelSize; i++)
				{
					read = ri+i;
					if (read>=position.x && read<size.x)
					{
						read+=yi;
						ca+=originalData[read] * kernel[i];
					}
				}
				ri = yi+xl;
				temp[ri] = ca/sum;;
			}
			yi+=width;
		}
		yi = static_cast<int>(position.y*width);

		//Vertical blur
		yl = static_cast<int>(position.y);
		nSizeY = static_cast<int>(size.y);
		for (; yl<nSizeY; yl++)
		{
			ym = yl-radius;
			riw = ym*width;
			xl = static_cast<int>(position.x);
			int nSizeX = static_cast<int>(size.x);

			for (; xl<nSizeX; xl++)
			{
				ca = 0;
				ri = ym;
				read = xl+riw;
				for (i = 0; i<kernelSize; i++)
				{
					if (ri<size.y && ri>=position.y)
						ca+=temp[read] * kernel[i];
					ri++;
					read+=width;
				}
				data[xl+yi] = ca/sum;
			}
			yi+=width;
		}
		//Free temp data	
		free(temp);
	}
}
//--------------------------------------------------------
FKCW_RenderEx_Texture2DMutable* FKCW_RenderEx_BlurTextureFilter::blur(FKCW_RenderEx_Texture2DMutable* texture, int radius) 
{
	return this->blur(texture, radius, CCRectZero);
}
//--------------------------------------------------------
FKCW_RenderEx_Texture2DMutable* FKCW_RenderEx_BlurTextureFilter::blur(FKCW_RenderEx_Texture2DMutable* texture, int radius, CCRect rect) 
{
	if(!texture)
		return NULL;
#if FKCW_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
	void *input = texture->getOriginalTexData();
#else
	void *input = texture->getTexData();
#endif
	CC_ASSERT( input != NULL );
	//Apply the effect to the texture
	this->blurInput(input, texture->getTexData(), texture->getPixelFormat(), texture->getPixelsWide(), texture->getPixelsHigh(), texture->getContentSize(), radius, rect); 
	//Update the GPU data
	texture->apply();
	return texture;
}
//--------------------------------------------------------