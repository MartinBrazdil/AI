#include "Pixels.h"

#include "Tensor.h"

namespace AI
{
	Pixels::Pixels(Tensor<>& tensor)
	{
		Write(tensor);
	}

	void Pixels::Write(Tensor<>& tensor)
	{
		SetSize(tensor.dims[0], tensor.dims[1]);
		Uint32* pixels = (Uint32*)front_buffer->pixels;
		for (size_t i = 0; i < tensor.Volume(2); i++)
		{
			Uint8 p = tensor.memory[i];
			pixels[i] = SDL_MapRGB(front_buffer->format, p, p, p);
		}
	}
}
