#include "Loader.hpp"

#include "Encoding.hpp"
#include "Error.hpp"
#include "Linker.hpp"
#include "String.hpp"

namespace SoftWire
{
	Loader::Loader(const Linker &linker) : linker(linker)
	{
		possession = true;
		finalized = false;

		machineCode = 0;
		instructions = 0;
		listing = 0;
	}

	Loader::~Loader()
	{
		if(possession)
		{
			delete[] machineCode;
			machineCode = 0;
		}

		delete instructions;
		instructions = 0;

		delete[] listing;
		listing = 0;
	}

	void (*Loader::callable(const char *entryLabel))()
	{
		if(finalized)
		{
			throw Error("Cannot retrieve callable from finalized code");
		}

		if(!machineCode)
		{
			loadCode();
		}

		if(!entryLabel)
		{
			return (void(*)())machineCode;
		}

		const unsigned char *entryPoint = resolveLocal(entryLabel);

		if(!entryPoint)
		{
			throw Error("Entry point '%s' not found", entryLabel);
		}

		return (void(*)())entryPoint;
	}

	void (*Loader::finalize(const char *entryLabel))()
	{
		if(!machineCode)
		{
			loadCode();
		}

		if(!entryLabel)
		{
			return (void(*)())machineCode;
		}

		const unsigned char *entryPoint = resolveLocal(entryLabel);

		if(!entryPoint)
		{
			throw Error("Entry point '%s' not found", entryLabel);
		}

		finalized = true;

		delete instructions;
		instructions = 0;

		delete[] listing;
		listing = 0;

		return (void(*)())entryPoint;
	}

	void *Loader::acquire()
	{
		possession = false;

		return machineCode;
	}

	void Loader::appendEncoding(const Encoding &encoding)
	{
		if(!instructions)
		{
			instructions = new Instruction();
		}

		instructions->append(encoding);
	}

	void Loader::loadCode(const char *entryLabel)
	{
		int length = codeLength();

		machineCode = new unsigned char[length + 16];   // NOTE: Code length is not accurate due to alignment issues
		unsigned char *currentCode = machineCode;

		Instruction *instruction = instructions;

		while(instruction)
		{
			Encoding &encoding = *instruction;
			encoding.setAddress(currentCode);
			const char *reference = encoding.getReference();
			const char *label = encoding.getLabel();

			if(reference)
			{
				if(encoding.relativeReference())
				{
					int offset = resolveReference(reference) - currentCode - encoding.length(currentCode);
					encoding.setJumpOffset(offset);
				}
				else
				{
					int address = (int)resolveReference(reference);

					// Encoded as memory reference or immediate?
					if(encoding.hasDisplacement())
					{
						encoding.addDisplacement(address);
					}
					else if(encoding.hasImmediate())
					{
						encoding.setImmediate(address);
					}
					else
					{
						INTERNAL_ERROR;
					}
				}
			}
			else if(encoding.hasImmediate() && encoding.relativeReference())
			{
				int offset = encoding.getImmediate() - (int)currentCode - encoding.length(currentCode);
				encoding.setCallOffset(offset);
			}

			currentCode += encoding.writeCode(currentCode);

			instruction = instruction->next();
		}
	}

	const unsigned char *Loader::resolveReference(const char *name) const
	{
		const unsigned char *reference = resolveLocal(name);
		
		if(reference)
		{
			return reference;
		}
		else
		{
			reference = resolveExternal(name);
		}

		if(reference)
		{
			return reference;
		}
		else
		{
			throw Error("Unresolved identifier '%s'", name);
		}
	}

	const unsigned char *Loader::resolveLocal(const char *name) const
	{
		const Instruction *instruction = instructions;

		unsigned char *target = machineCode;

		while(instruction)
		{
			if(instruction->getLabel() && strcmp(instruction->getLabel(), name) == 0)
			{
				return target;
			}
			else
			{
				target += instruction->length(target);
			}

			instruction = instruction->next();
		}

		return 0;
	}

	const unsigned char *Loader::resolveExternal(const char *name) const
	{
		return (unsigned char*)linker.resolveExternal(name);
	}

	int Loader::codeLength() const
	{
		const Instruction *instruction = instructions;
		unsigned char *end = machineCode;

		while(instruction)
		{
			end += instruction->length(end);

			instruction = instruction->next();
		}

		return end - machineCode;
	}

	const char *Loader::getListing()
	{
		if(!machineCode)
		{
			return 0;
		}

		if(listing)
		{
			return listing;
		}

		listing = new char[codeLength() * 4];
		char *buffer = listing;

		for(Instruction *instruction = instructions; instruction; instruction = instruction->next())
		{
			buffer += instruction->printCode(buffer);
		}

		*(buffer - 1) = '\0';

		return listing;
	}

	void Loader::clearListing()
	{
		delete[] listing;
		listing = 0;
	}
}
