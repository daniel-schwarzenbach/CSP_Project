/*
    Light Weight Dynamic Memory Allocation Tracker Library
    Author:  Justin C Presley
    Github:  https://github.com/justincpresley
    Contact: justincpresley@gmail.com
*/

#ifndef ALLOC_TRACK_H
#define ALLOC_TRACK_H

#include <iostream>

namespace mem {
	// accessible library functions
	static void debuggerPrintMissingAllocations();
}

namespace mem {
	namespace bug {
		// debugger variables
		struct allocation{
		  void*         addr;
		  size_t        size;
		  const char*   file;
		  unsigned int  line;
		  unsigned int  when;
		};
		typedef struct allocation allocation;

		static constexpr unsigned long long int defaultAllocArraySize = sizeof(allocation);
		static unsigned long long int           allocArraySize        = defaultAllocArraySize;
		static unsigned long long int           numCurrentAlloc       = 0;
		static unsigned long long int           numTotalAlloc         = 0;
		static unsigned long long int           memoryCurrentSize     = 0;
		static unsigned long long int           memoryHighestSize     = 0;
		static unsigned long long int           memoryTotalSize       = 0;
		static allocation**                     allocArray            = nullptr;

		// debugger functions
		static void updateDebugerOfNew(void*& ptr,std::size_t sz,const char* fi,unsigned int ln);
		static void updateDebugerOfDelete(void*& ptr);

		static void pushToAllocArray(allocation*& newAlloc);
		static void popFromAllocArray(allocation*& oldAlloc);

		inline static allocation* allocArrayFind(void*& ptr);
		inline static allocation* allocationCreate(void*& a,std::size_t s,const char* f,unsigned int l, unsigned int w);
		inline static void        allocationPrint(allocation*& alloc, short int type);
	}
}

// new overloads
void* operator new(const std::size_t sz,const char* fi,const unsigned int ln){
	void *ptr = std::malloc(sz);
	mem::bug::updateDebugerOfNew(ptr,sz,fi,ln);
	if(ptr){return ptr;}else{throw std::bad_alloc{};} }
void* operator new[](const std::size_t sz,const char* fi,const unsigned int ln){
	void *ptr = std::malloc(sz);
	mem::bug::updateDebugerOfNew(ptr,sz,fi,ln);
	if(ptr){return ptr;}else{throw std::bad_alloc{};} }
#define new new(__FILE__,__LINE__)

// delete overloads
void operator delete(void* ptr) noexcept{
	mem::bug::updateDebugerOfDelete(ptr);
	std::free(ptr); }
void operator delete[](void* ptr) noexcept{
	mem::bug::updateDebugerOfDelete(ptr);
	std::free(ptr); }
void operator delete (void* ptr, std::size_t size) noexcept{
	mem::bug::updateDebugerOfDelete(ptr);
	std::free(ptr); }
void operator delete[](void* ptr, std::size_t size) noexcept{
	mem::bug::updateDebugerOfDelete(ptr);
	std::free(ptr); }

// updating the debugger
static void mem::bug::updateDebugerOfNew(void*& ptr,std::size_t sz,const char* fi, unsigned int ln){
	mem::bug::allocation* newAlloc = mem::bug::allocationCreate(ptr, sz, fi, ln, mem::bug::numTotalAlloc);
	mem::bug::numCurrentAlloc++;
	mem::bug::numTotalAlloc++;
	mem::bug::memoryTotalSize += static_cast<unsigned long long int>( newAlloc->size );
	mem::bug::memoryCurrentSize += static_cast<unsigned long long int>( newAlloc->size );
	mem::bug::memoryHighestSize = (mem::bug::memoryCurrentSize>mem::bug::memoryHighestSize) ? mem::bug::memoryCurrentSize : mem::bug::memoryHighestSize;
	mem::bug::allocationPrint(newAlloc, 1);
	mem::bug::pushToAllocArray(newAlloc);
	newAlloc = nullptr;
}
static void mem::bug::updateDebugerOfDelete(void*& ptr){
	mem::bug::allocation* oldAlloc = mem::bug::allocArrayFind(ptr);
	if(oldAlloc != nullptr){
		mem::bug::numCurrentAlloc--;
		mem::bug::memoryCurrentSize -= static_cast<unsigned long long int>( oldAlloc->size );
		mem::bug::allocationPrint(oldAlloc, -1);
		mem::bug::popFromAllocArray(oldAlloc);
		oldAlloc = nullptr;
	}
}

// debugger functions
static void mem::bug::pushToAllocArray(mem::bug::allocation*& newAlloc){
	if(mem::bug::allocArray == nullptr){
		mem::bug::allocArray = (mem::bug::allocation**)calloc(mem::bug::allocArraySize/sizeof(mem::bug::allocation), sizeof(mem::bug::allocation*));
		for(unsigned long long int i=0; i<mem::bug::allocArraySize/sizeof(mem::bug::allocation); i++){
			mem::bug::allocArray[i] = nullptr;
		}
	}
	if(mem::bug::allocArraySize <= mem::bug::numCurrentAlloc*sizeof(mem::bug::allocation*)){
		mem::bug::allocation** tempArray = (mem::bug::allocation**)calloc(mem::bug::allocArraySize/sizeof(mem::bug::allocation), sizeof(mem::bug::allocation*));
		for(unsigned long long int i=0; i<(mem::bug::allocArraySize/sizeof(mem::bug::allocation)); i++){tempArray[i] = mem::bug::allocArray[i];}
		std::free(mem::bug::allocArray);

		mem::bug::allocArraySize *= 2;
		mem::bug::allocArray = (mem::bug::allocation**)calloc(mem::bug::allocArraySize/sizeof(mem::bug::allocation), sizeof(mem::bug::allocation*));
		for(unsigned long long int i=0; i<(mem::bug::allocArraySize)/sizeof(mem::bug::allocation); i++){mem::bug::allocArray[i] = nullptr;}
		for(unsigned long long int i=0; i<(mem::bug::allocArraySize/2)/sizeof(mem::bug::allocation); i++){mem::bug::allocArray[i] = tempArray[i];}
		std::free(tempArray);
	}
	mem::bug::allocArray[numCurrentAlloc-1] = newAlloc;
}
static void mem::bug::popFromAllocArray(mem::bug::allocation*& oldAlloc){
	oldAlloc->addr = nullptr;
	bool allNulls = true;
	unsigned long long int pos = 0;

	for(unsigned long long int i=0; i<mem::bug::numCurrentAlloc+1; i++){
		if(mem::bug::allocArray[i] == oldAlloc){
			mem::bug::allocArray[i] = nullptr;
			pos = i;
			std::free(oldAlloc);
		}else{
			if(mem::bug::allocArray[i] != nullptr){ allNulls = false; }
		}
	}

	if(allNulls){
		std::free(mem::bug::allocArray);
		mem::bug::allocArray = nullptr;
		mem::bug::allocArraySize = mem::bug::defaultAllocArraySize;
	}else{
		for(unsigned long long int i=pos; i<mem::bug::numCurrentAlloc+1; i++){
			if(mem::bug::allocArray[i+1] == nullptr){
				mem::bug::allocArray[i] = nullptr;
			}else{
				mem::bug::allocArray[i] = mem::bug::allocArray[i+1];
				mem::bug::allocArray[i+1] = nullptr;
			}
		}
	}
}
inline static mem::bug::allocation* mem::bug::allocArrayFind(void*& ptr){
	if(mem::bug::allocArray != nullptr){
		for(unsigned long long int i=0; i<mem::bug::numCurrentAlloc; i++){
			if(mem::bug::allocArray[i] != nullptr){
				if(mem::bug::allocArray[i]->addr == ptr){
					return mem::bug::allocArray[i];
				}
			}
		}
		return nullptr;
	}else{
		return nullptr;
	}
}
inline static mem::bug::allocation* mem::bug::allocationCreate(void*& a,std::size_t s,const char* f,unsigned int l, unsigned int w){
	mem::bug::allocation* alloc = (mem::bug::allocation*)malloc(sizeof(mem::bug::allocation));
	alloc->addr = a;
	alloc->size = s;
	alloc->file = f;
	alloc->line = l;
	alloc->when = w;
	return alloc;
}
inline static void mem::bug::allocationPrint(mem::bug::allocation*& alloc, short int type){
	std::cerr<< alloc->when <<" | "<< alloc->file <<"(line "<< alloc->line <<")\n";
	if(type == 1){
		std::cerr<<"\tcreating alloc (address:"<< alloc->addr <<" | size:"<< alloc->size <<" bytes)\n";
	}else if(type==-1){
		std::cerr<<"\tdeleting alloc (address:"<< alloc->addr <<" | size:"<< alloc->size <<" bytes)\n";
	}else{
		std::cerr<<"\talloc (address:"<< alloc->addr <<" | size:"<< alloc->size <<" bytes)\n";
	}
}

// start of accessible library functions
static void mem::debuggerPrintMissingAllocations(){
	std::cout << std::endl;
	std::cout << "# Allocations left: " << mem::bug::numCurrentAlloc << std::endl;
	std::cout << "Not deallocated: " << std::endl;
	if(mem::bug::allocArray != nullptr){
		for(unsigned long long int i=0; i<(mem::bug::numCurrentAlloc); i++){
			if(mem::bug::allocArray[i] != nullptr){
				std::cout << "\taddr:" << mem::bug::allocArray[i]->addr << " - size:" << mem::bug::allocArray[i]->size << std::endl;
			}
		}
	}else{
		std::cout << "\tNone" << std::endl;
	}
	std::cout << std::endl;
}

#endif//ALLOC_TRACK_H