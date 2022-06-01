#ifndef LIBDATACHANNELS_INTERNAL_BUFFER_H_
#define	LIBDATACHANNELS_INTERNAL_BUFFER_H_
#include <stdint.h>
#include <stddef.h>
#include <cstdlib>
#include <cstring>
#include <utility>


//Allingment must be a power of 2
inline int RoundUp(size_t alignment, size_t size) 
{
   return size ? (size + alignment - 1) & -alignment : 0;
}

class Buffer
{
public:
        Buffer(const uint8_t* data, const size_t size)
        {
                //Set buffer size to a
                capacity = RoundUp(64,size);
                //Allocate memory
#ifdef HAVE_STD_ALIGNED_ALLOC
                buffer = (uint8_t*) std::aligned_alloc(64, this->capacity);
#else
                buffer = (uint8_t*) std::malloc(this->capacity);
#endif
                //Copy
                memcpy(buffer,data,size);
                //Reset size
                this->size = size;
        }

        Buffer(size_t capacity = 0)
        {
                //Set buffer size
                this->capacity = RoundUp(64,capacity);
#ifdef HAVE_STD_ALIGNED_ALLOC
                //Allocate memory
                buffer = capacity ? (uint8_t*) std::aligned_alloc(64, this->capacity) : nullptr;
#else
                buffer = capacity ? (uint8_t*) std::malloc(this->capacity) : nullptr;
#endif
                //NO size
                this->size = 0;
        }
	
	Buffer(Buffer &&other) noexcept
	{
		this->capacity = other.capacity;
		this->buffer = other.buffer;
		this->size = other.size;
		other.buffer = nullptr;
		other.capacity = 0;
		other.size = 0;
	}
	
	Buffer& operator=(Buffer&& other) noexcept
	{
		std::free(this->buffer);
		this->capacity = other.capacity;
		this->buffer = other.buffer;
		this->size = other.size;
		other.buffer = nullptr;
		other.capacity = 0;
		other.size = 0;
		return *this; 
	}
	
	//Not copiable
	Buffer(const Buffer &) = delete;
	Buffer& operator=(const Buffer&) = delete;
	
	~Buffer()
	{
		std::free(buffer);
	}
	
	uint8_t* GetData() const		{ return buffer;		}
	size_t GetCapacity() const		{ return capacity;		}
	size_t GetSize() const			{ return size;			}

	void SetSize(size_t size) 
	{ 
		//Check capacity
		if (size>capacity)
			//Allocate new size
			Alloc(size);
		//Set new size
		this->size = size;
	}

	void Alloc(size_t capacity)
	{
		//Calculate new size
		this->capacity = capacity;
		//Realloc
		buffer = ( uint8_t*) std::realloc(buffer,capacity);
		//Check new size
		if (size>capacity)
			//reduce size
			size = capacity;
	}

	void SetData(const uint8_t* data,const size_t size)
	{
		//Check size
		if (size>capacity)
			//Allocate new size
			Alloc(size);
		//Copy
		std::memcpy(buffer,data,size);
		//Reset size
		this->size = size;
	}
	
	void SetData(const Buffer& buffer)
	{
		SetData(buffer.GetData(),buffer.GetSize());
	}

	void AppendData(const uint8_t* data,const size_t size)
	{
		//Check size
		if (this->size+size>capacity)
			//Allocate new size
			Alloc(this->size+size);
		//Copy
		std::memcpy(buffer+this->size,data,size);
		//Increase size
		this->size += size;
	}
	
	Buffer Clone() const
	{
		return Buffer(buffer,size);
	}
	
	static Buffer&& Wrap(uint8_t* data, size_t size)
	{
		Buffer buffer;
		
		buffer.buffer = data;
		buffer.capacity = size;
		buffer.size = size;
		
		return std::move(buffer);
	}
	
protected:
	uint8_t* buffer		= nullptr;
	size_t capacity		= 0;
	size_t size		= 0;
};

#endif

