#include "pch.h"
#include "Datum.h"
#include "Scope.h"
#include "RTTI.h"

using namespace Fiea::GameEngine;
typedef Fiea::GameEngine::Datum Datum;
typedef std::string StdString;
Datum::Datum() {
	isExtern = false;
	Size = 0;
	Capacity = 0;
	_datatype = DatumType::Unknown;
}
//copy constructors
Datum::Datum(const Datum& other):Size(other.Size),Capacity(other.Capacity),_datatype(other._datatype),isExtern(other.isExtern){
	if (other.isExtern == true) {
		isExtern = false;
	}

		switch (other._datatype) {
		case DatumType::Int:
			_data.intData = (int*)malloc(Capacity * sizeof(int));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.intData + i) int(other._data.intData[i]);
			}
			break;
		case DatumType::Float:
			_data.floatData = (float*)malloc(Capacity * sizeof(float));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.floatData + i) float(other._data.floatData[i]);
			}
			break;
		case DatumType::String:
			_data.stringData = (StdString*)malloc(Capacity * sizeof(StdString));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.stringData + i) StdString(other._data.stringData[i]);
			}
			break;
		case DatumType::Vec4:
			_data.vecData = (glm::vec4*)malloc(Capacity * sizeof(glm::vec4));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.vecData + i) glm::vec4(other._data.vecData[i]);
			}
			break;
		case DatumType::Mat4:
			_data.matData = (glm::mat4x4*)malloc(Capacity * sizeof(glm::mat4x4));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.matData + i) glm::mat4x4(other._data.matData[i]);
			}
			break;
		case DatumType::Table:
			_data.scopeData = (Scope**)malloc(Capacity * sizeof(Scope*));
			break;
		case DatumType::Pointer:
			_data.pointerData = (RTTI**)malloc(Capacity * sizeof(RTTI*));
			break;
		}
}
//move constructor
Datum::Datum(Datum&& other) noexcept : Size(other.Size), Capacity(other.Capacity), _datatype(other._datatype), isExtern(other.isExtern)
{
	//if external memory is involed, the memory address is merely handed off
	if (isExtern) {
		switch (_datatype) {
		case DatumType::Int:
			_data.intData = other._data.intData;
			other._data.intData = nullptr;
			break;
		case DatumType::Float:
			_data.floatData = other._data.floatData;
			other._data.floatData = nullptr;
			break;
		case DatumType::String:
			_data.stringData = other._data.stringData;
			other._data.stringData = nullptr;
			break;
		case DatumType::Vec4:
			_data.vecData = other._data.vecData;
			other._data.vecData = nullptr;
			break;
		case DatumType::Mat4:
			_data.matData = other._data.matData;
			other._data.matData = nullptr;
			break;
		}
	}
	//if not, the memory is moved
	else {
		switch (other._datatype) {
		case DatumType::Int:
			_data.intData = (int*)malloc(Size * sizeof(int));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.intData + i) int(other._data.intData[i]);
			}
			break;
		case DatumType::Float:
			_data.floatData = (float*)malloc(Size * sizeof(float));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.floatData + i) float(other._data.floatData[i]);
			}
			break;
		case DatumType::String:
			_data.stringData = (StdString*)malloc(Size * sizeof(StdString));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.stringData + i) StdString(other._data.stringData[i]);
			}
			break;
		case DatumType::Vec4:
			_data.vecData = (glm::vec4*)malloc(Size * sizeof(glm::vec4));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.vecData + i) glm::vec4(other._data.vecData[i]);
			}
			break;
		case DatumType::Mat4:
			_data.matData = (glm::mat4x4*)malloc(Size * sizeof(glm::mat4x4));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.matData + i) glm::mat4x4(other._data.matData[i]);
			}
			break;
		}
	}
}
Datum::~Datum() {
	if (!isExtern) {
		switch (_datatype) {
		case DatumType::Int:
			free(_data.intData);
			_data.intData = nullptr;
			break;
		case DatumType::Float:
			free(_data.floatData);
			_data.floatData = nullptr;
			break;
		case DatumType::String:
			for (size_t i = 0; i < Size; ++i) {
				_data.stringData[i].~basic_string();
			}
			free(_data.stringData);
			_data.stringData = nullptr;
			break;
		case DatumType::Vec4:
			free(_data.vecData);
			_data.vecData = nullptr;
			break;
		case DatumType::Mat4:
			free(_data.matData);
			_data.matData = nullptr;
			break;
		case DatumType::Table:
			free(_data.scopeData);
			_data.scopeData = nullptr;
			break;
		case DatumType::Pointer:
			free(_data.pointerData);
			_data.pointerData = nullptr;
			break;
		}
	}
}




//Reserve memory functionality
void Datum::Reserve(size_t NewSize) {
	if (!isExtern)
	{
		int* newArray = nullptr;
		float* newfloatArray = nullptr;
		std::string* newStrArray = nullptr;
		glm::vec4* newVecArray = nullptr;
		glm::mat4x4* newMatArray = nullptr;
		Scope** newscopeList = nullptr;
		RTTI** newpointerList = nullptr;
		//If capacity already there do nothing
		if (NewSize < Capacity) {
			return;
		}
		//else, allocate a new array and move items over
		else {
			switch (_datatype) {
			case DatumType::Int:
				newArray = (int*)malloc(NewSize * sizeof(int));
				for (size_t i = 0; i < Size; ++i) {
					new(newArray + i) int(_data.intData[i]);
				}
				free(_data.intData);
				_data.intData = newArray;
				break;
			case DatumType::Float:
				newfloatArray = (float*)malloc(NewSize * sizeof(float));
				for (size_t i = 0; i < Size; ++i) {
					new(newfloatArray + i) float(_data.floatData[i]);
				}
				free(_data.floatData);
				_data.floatData = newfloatArray;
				break;
			case DatumType::String:
				newStrArray = (StdString*)malloc(NewSize * sizeof(StdString));
				for (size_t i = 0; i < Size; ++i) {
					new(newStrArray + i) StdString(_data.stringData[i]);
					_data.stringData[i].~basic_string();
				}
				free(_data.stringData);
				_data.stringData = newStrArray;
				break;
			case DatumType::Vec4:
				newVecArray = (glm::vec4*)malloc(NewSize * sizeof(glm::vec4));
				for (size_t i = 0; i < Size; ++i) {
					new(newVecArray + i) glm::vec4(_data.vecData[i]);
				}
				free(_data.vecData);
				_data.vecData = newVecArray;
				break;
			case DatumType::Mat4:
				newMatArray = (glm::mat4x4*)malloc(NewSize * sizeof(glm::mat4x4));
				for (size_t i = 0; i < Size; ++i) {
					new(newMatArray + i)glm::mat4x4(_data.matData[i]);
				}
				free(_data.matData);
				_data.matData = newMatArray;
				break;
			case DatumType::Table:
				newscopeList = (Scope**)malloc(sizeof(Scope*) * NewSize);
				for (size_t i = 0; i < Size; ++i) {
					newscopeList[i] = _data.scopeData[i];
				}
				free(_data.scopeData);
				_data.scopeData = newscopeList;
				break;
			case DatumType::Pointer:
				newpointerList = (RTTI**)malloc(sizeof(RTTI*) * NewSize);
				for (size_t i = 0; i < Size; ++i) {
					newpointerList[i] = _data.pointerData[i];
				}
				free(_data.pointerData);
				_data.pointerData = newpointerList;
				break;
			}
			Capacity = NewSize;
		}
	}
	else
		throw std::logic_error("You do not own this memory");
}

//pop
void Datum::PopBack() {
	int* newArray;
	float* newfloatArray;
	StdString* newstrArray;
	glm::mat4x4* newMatArray;
	glm::vec4* newVecArray;
	if (Size == 0|| _datatype == Unknown) {
		throw std::logic_error("Nothing to pop");
	}
	else if (isExtern) {
		throw std::logic_error("Popping is not possible on excternal memory");
	}
	else {
		switch (_datatype) {
		case Int: {
			newArray = (int*)malloc(sizeof(int) * Capacity);
			for (size_t i = 0; i < (Size); i++) {
				if(i!=Size - 1)
					new(newArray + i)int(_data.intData[i]);

			}
			free(_data.intData);
			_data.intData = newArray;
			Size -= 1;
			break;
			}
		case Float:
			newfloatArray = (float*)malloc(sizeof(float) * Capacity);
			for (size_t i = 0; i < Size; ++i) {
				if (i != Size - 1)
					new(newfloatArray + i)float(_data.floatData[i]);
			}
			free(_data.floatData);
			_data.floatData = newfloatArray;
			Size -= 1;
			break;
		case String:
			newstrArray = (StdString*)malloc(sizeof(StdString) * Capacity);
			for (size_t i = 0; i < Size; ++i) {
				if (i != Size - 1)
					new(newstrArray + i)StdString(_data.stringData[i]);
				_data.stringData[i].~basic_string();
			}
			free(_data.stringData);
			_data.stringData = newstrArray;
			Size -= 1;
			break;
		case Vec4:
			newVecArray = (glm::vec4*)malloc(Capacity * sizeof(glm::vec4));
			for (size_t i = 0; i < Size; ++i)
			{
				if (i != Size - 1)
					new(newVecArray + i)glm::vec4(_data.vecData[i]);
			}
			free(_data.vecData);
			_data.vecData = newVecArray;
			Size -= 1;
			break;
		case Mat4:
			newMatArray = (glm::mat4x4*)malloc(Capacity * sizeof(glm::mat4x4));
			for (size_t i = 0; i < Size; ++i) {
				if (i != Size - 1)
					new(newMatArray + i)glm::mat4x4(_data.matData[i]);
			}
			free(_data.matData);
			_data.matData = newMatArray;
			Size -= 1;
			break;
		}
	}
}
//returns data as string
std::string Datum::GetItemAsString(size_t idx) {
	if (idx >= Size || idx < 0)
		throw std::out_of_range("Index out of range");
	if (_datatype == Unknown)
		throw std::logic_error("Untyped Datum");
	if (_datatype == Int) {
		return std::to_string(_data.intData[idx]);
	}
	else if (_datatype == Float) {
		return std::to_string(_data.floatData[idx]);
	}
	else if (_datatype == String) {
		return _data.stringData[idx];
	}
	else if (_datatype == Vec4) {
		return THelper.vec4ToString(_data.vecData[idx]);
	}
	else if (_datatype == Mat4) {
		return THelper.mat4x4ToString(_data.matData[idx]);
	}
	return std::string("");
}

void Fiea::GameEngine::Datum::PushItem(RTTI* item)
{
	if (isExtern) {
		throw std::logic_error("Invalid Operation");
	}
	if (_datatype == Pointer || _datatype == Unknown) {
		_datatype = Pointer;
		if (Size == 0 || Size == Capacity) {
			Reserve(Size + 1);
			_data.pointerData[Size] = item;
			++Size;
		}
		else if (Size < Capacity) {
			_data.pointerData[Size] = item;
			++Size;
		}
	}
	else {
		throw std::logic_error("Type Mismatch");
	}
}


//equality operator
bool Datum::operator==(const Datum& rhs) const {
	if (_datatype != rhs._datatype) {
		return false;
	}
	else {
		if (Size != rhs.Size) {
			return false;
		}
		else {
			switch (_datatype) {
			case Int: 
				for (size_t i = 0; i < Size; ++i) {
					if (_data.intData[i] != rhs._data.intData[i]) {
						return false;
					}
				}
				return true;
				break;
			case Float:
				for (size_t i = 0; i < Size; ++i) {
					if (_data.floatData[i] != rhs._data.floatData[i]) {
						return false;
					}
				}
				return true;
				break;
			case String:
				for (size_t i = 0; i < Size; ++i) {
					if (_data.stringData[i] != rhs._data.stringData[i]) {
						return false;
					}
				}
				return true;
				break;
			case Vec4:
				for (size_t i = 0; i < Size; ++i) {
					if (_data.vecData[i] != rhs._data.vecData[i]) {
						return false;
					}
				}
				return true;
				break;
			case Mat4:
				for (size_t i = 0; i < Size; ++i) {
					if(_data.matData[i] != rhs._data.matData[i]){
						return false;
					}
				}
				return true;
				break;
			case Table:
				for (size_t i = 0; i < Size; ++i) {
					if (*(_data.scopeData[i]) != *(rhs._data.scopeData[i]))
						return false;
				}
				return true;
				break;
			case Unknown:
				return true;
				break;
			}
		}

	}
	return false;
}

//not equal operator
bool Datum::operator!=(const Datum& rhs) const {
	bool flag = false;
	if (_datatype != rhs._datatype) {
		return true;
	}
	else {
		switch (_datatype) {
		case Int: 
			for (size_t i = 0; i < Size; ++i) {
				if (_data.intData[i] != rhs._data.intData[i])
					flag = true;
			}
			break;
		case Float:
			for (size_t i = 0; i < Size; ++i) {
				if (_data.floatData[i] != rhs._data.floatData[i])
					flag = true;
			}
			break;
		case String:
			for (size_t i = 0; i < Size; ++i) {
				if (_data.stringData[i] != rhs._data.stringData[i])
					flag = true;
			}
			break;
		case Vec4:
			for (size_t i = 0; i < Size; ++i) {
				if (_data.vecData[i] != rhs._data.vecData[i])
					flag = true;
			}
			break;
		case Mat4:
			for (size_t i = 0; i < Size; ++i) {
				if (_data.matData[i] != rhs._data.matData[i])
					flag = true;
			}
			break;
		case Table:
			return !(operator==(rhs));
			break;
		}
	}
	return flag;
}

//copy assignment
Datum& Datum::operator=(const Datum& other) {
	if (this != &other) {
		if (this->_datatype == Unknown || this->_datatype == other._datatype) {
			clear();
			_datatype = other._datatype;
			Size = other.Size;
			Capacity = other.Capacity;
			switch (_datatype) {
			case DatumType::Int:
				_data.intData = (int*)malloc(Capacity * sizeof(int));
				for (size_t i = 0; i < Size; ++i) {
					new(_data.intData + i) int(other._data.intData[i]);
				}
				break;
			case DatumType::Float:
				_data.floatData = (float*)malloc(Capacity * sizeof(float));
				for (size_t i = 0; i < Size; ++i) {
					new(_data.floatData + i) float(other._data.floatData[i]);
				}
				break;
			case DatumType::String:
				_data.stringData = (StdString*)malloc(Capacity * sizeof(StdString));
				for (size_t i = 0; i < Size; ++i) {
					new(_data.stringData + i) StdString(other._data.stringData[i]);
				}
				break;
			case DatumType::Vec4:
				_data.vecData = (glm::vec4*)malloc(Capacity * sizeof(glm::vec4));
				for (size_t i = 0; i < Size; ++i) {
					new(_data.vecData + i) glm::vec4(other._data.vecData[i]);
				}
				break;
			case DatumType::Mat4:
				_data.matData = (glm::mat4x4*)malloc(Capacity * sizeof(glm::mat4x4));
				for (size_t i = 0; i < Size; ++i) {
					new(_data.matData + i) glm::mat4x4(other._data.matData[i]);
				}
				break;
			case DatumType::Table:
				_data.scopeData = (Scope**)malloc(Capacity * sizeof(Scope*));
				for (size_t i = 0; i < Size; ++i) {
					new(_data.scopeData + i)Scope* (other._data.scopeData[i]);
				}
				break;
			}

		}
		else {
			throw std::logic_error("Type mismatch");
		}
	}
	return *this;
}


//move assignment operator
Datum& Datum::operator=(Datum&& other) noexcept{
	if(_datatype!= Unknown)
		clear();
	Size = other.Size;
	Capacity = other.Capacity;
	_datatype = other._datatype;
	//if external memory is involed, the memory address is merely handed off
	if (other.isExtern) {
		switch (_datatype) {
		case DatumType::Int:
			_data.intData = other._data.intData;
			other._data.intData = nullptr;
			break;
		case DatumType::Float:
			_data.floatData = other._data.floatData;
			other._data.floatData = nullptr;
			break;
		case DatumType::String:
			_data.stringData = other._data.stringData;
			other._data.stringData = nullptr;
			break;
		case DatumType::Vec4:
			_data.vecData = other._data.vecData;
			other._data.vecData = nullptr;
			break;
		case DatumType::Mat4:
			_data.matData = other._data.matData;
			other._data.matData = nullptr;
			break;
		}
	}
	//if not, the memory is moved
	else {
		switch (other._datatype) {
		case DatumType::Int:
			_data.intData = (int*)malloc(Capacity * sizeof(int));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.intData + i) int(other._data.intData[i]);
			}
			break;
		case DatumType::Float:
			_data.floatData = (float*)malloc(Capacity * sizeof(float));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.floatData + i) float(other._data.floatData[i]);
			}
			break;
		case DatumType::String:
			_data.stringData = (StdString*)malloc(Capacity * sizeof(StdString));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.stringData + i) StdString(other._data.stringData[i]);
			}
			break;
		case DatumType::Vec4:
			_data.vecData = (glm::vec4*)malloc(Capacity * sizeof(glm::vec4));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.vecData + i) glm::vec4(other._data.vecData[i]);
			}
			break;
		case DatumType::Mat4:
			_data.matData = (glm::mat4x4*)malloc(Capacity * sizeof(glm::mat4x4));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.matData + i) glm::mat4x4(other._data.matData[i]);
			}
			break;
		case DatumType::Table:
			_data.scopeData = (Scope**)malloc(Capacity * sizeof(Scope*));
			for (size_t i = 0; i < Size; ++i) {
				new(_data.scopeData + i)Scope* (other._data.scopeData[i]);
			}
			break;
		}
	}
	return *this;
}


//Returns the type datum is set to
Fiea::GameEngine::DatumType Datum::GetType() const{
	return _datatype;
}



//Clear all information
void Datum::clear() {
	if (isExtern) {
		
		switch (_datatype) {
		case Int:
			_data.intData=nullptr;
			break;
		case Float:
			_data.floatData = nullptr;
			break;
		case String:
			_data.stringData = nullptr;
			break;
		case Vec4:
			_data.vecData = nullptr;
			break;
		case Mat4:
			_data.matData = nullptr;
			break;
		case Pointer:
			_data.pointerData = nullptr;
			break;
		case Table:
			_data.scopeData = nullptr;
			break;
		}
	}
	else
		switch (_datatype) {
		case Int: 
			free(_data.intData);
			break;
		case Float:
			free(_data.floatData);
			break;
		case String:
			for (size_t i = 0; i < Size; ++i) {
				_data.stringData[i].~basic_string();
			}
			break;
		case Vec4:
			free(_data.vecData);
			break;
		case Mat4:
			free(_data.matData);
			break;
		case Table:
			free(_data.scopeData);
			break;
		case Pointer:
			free(_data.pointerData);
			break;
		}
	Size = 0;
	Capacity = 0;
	_datatype = Unknown;
	isExtern = false;
}

//shrink to fit functionality
void Datum::shrink_to_fit() {
	if (isExtern) {
		throw std::exception("Externally owned. No memory operations permitted");
	}
	int* newIntArray;
	float* newfloatArray;
	StdString* newStrArray;
	glm::vec4* newVecArray;
	glm::mat4x4* newMatArray;
	if (Size == Capacity) {
		return;
	}
	else {
		switch (_datatype) {
		case Int:
			newIntArray = (int*)malloc(sizeof(int) * Size);
			for (size_t i = 0; i < Size; ++i) {
				new(newIntArray + i) int(_data.intData[i]);
			}
			free(_data.intData);
			_data.intData = newIntArray;
			Capacity = Size;
			break;
		case Float:
			newfloatArray = (float*)malloc(sizeof(float) * Size);
			for (size_t i = 0; i < Size; ++i) {
				new(newfloatArray + i)float(_data.floatData[i]);
			}
			free(_data.floatData);
			_data.floatData = newfloatArray;
			Capacity = Size;
			break;
		case Vec4:
			newVecArray = (glm::vec4*)malloc(sizeof(glm::vec4) * Size);
			for (size_t i = 0; i < Size; ++i) {
				new(newVecArray + i)glm::vec4(_data.vecData[i]);
			}
			free(_data.vecData);
			_data.vecData = newVecArray;
			Capacity = Size;
			break;
		case String:
			newStrArray = (StdString*) malloc(sizeof(StdString) * Size);
			for (size_t i = 0; i < Size; ++i) {
				new(newStrArray + i)StdString(_data.stringData[i]);
				_data.stringData[i].~basic_string();
			}
			free(_data.stringData);
			_data.stringData = newStrArray;
			Capacity = Size;
			break;
		case Mat4:
			newMatArray = (glm::mat4x4*)malloc(sizeof(glm::mat4x4) * Size);
			for (size_t i = 0; i < Size; ++i) {
				new(newMatArray + i)glm::mat4x4(_data.matData[i]);
			}
			free(_data.matData);
			_data.matData = newMatArray;
			Capacity = Size;
			break;
		}

	}
}
//Capacity of datum: How much information can be saved here?
size_t Datum::GetCapacity() const {
	return Capacity;
}
//Size of datum: How much actual data is in datum?
size_t Datum::GetSize() const{
	return Size;
}

void Datum::Resize(size_t NewSize) {
	if (isExtern) {
		throw std::exception("Externally owned memory");
	}
	int* newIntArray;
	float* newfloatArray;
	StdString* newStrArray;
	glm::vec4* newVecArray;
	glm::mat4x4* newMatArray;
	Size = Capacity = NewSize;
	switch (_datatype) {
	case Int:
		newIntArray = (int*)malloc(sizeof(int) * Size);
		for (size_t i = 0; i < Size; ++i) {
			new(newIntArray + i) int(_data.intData[i]);
		}
		free(_data.intData);
		_data.intData = newIntArray;
		Capacity = Size;
		break;
	case Float:
		newfloatArray = (float*)malloc(sizeof(float) * Size);
		for (size_t i = 0; i < Size; ++i) {
			new(newfloatArray + i)float(_data.floatData[i]);
		}
		free(_data.floatData);
		_data.floatData = newfloatArray;
		Capacity = Size;
		break;
	case Vec4:
		newVecArray = (glm::vec4*)malloc(sizeof(glm::vec4) * Size);
		for (size_t i = 0; i < Size; ++i) {
			new(newVecArray + i)glm::vec4(_data.vecData[i]);
		}
		free(_data.vecData);
		_data.vecData = newVecArray;
		Capacity = Size;
		break;
	case String:
		newStrArray = (StdString*)malloc(sizeof(StdString) * Size);
		for (size_t i = 0; i < Size; ++i) {
			new(newStrArray + i)StdString(_data.stringData[i]);
			_data.stringData[i].~basic_string();
		}
		free(_data.stringData);
		_data.stringData = newStrArray;
		Capacity = Size;
		break;
	case Mat4:
		newMatArray = (glm::mat4x4*)malloc(sizeof(glm::mat4x4) * Size);
		for (size_t i = 0; i < Size; ++i) {
			new(newMatArray + i)glm::mat4x4(_data.matData[i]);
		}
		free(_data.matData);
		_data.matData = newMatArray;
		Capacity = Size;
		break;
	}
}

Datum& Datum::operator=(int&& scalar) {
	if (_datatype != Unknown) {
		throw std::logic_error("Invalid Operation");
	}
	_datatype = Int;
	PushItem<int>(scalar);
	return *this;
}

Datum& Datum::operator=(float&& scalar) {
	if (_datatype != Unknown) {
		throw std::logic_error("Invalid Operation");
	}
	_datatype = Float;
	PushItem<float>(scalar);
	return *this;
}

Datum& Datum::operator=(std::string&& scalar) {
	if (_datatype != Unknown) {
		throw std::logic_error("Invalid Operation");
	}
	_datatype = String;
	PushItem<std::string>(scalar);
	return *this;
}

Datum& Datum::operator=(glm::vec4&& scalar) {
	if (_datatype != Unknown) {
		throw std::logic_error("Invalid Operation");
	}
	_datatype = Vec4;
	PushItem<glm::vec4>(scalar);
	return *this;
}

Datum& Datum::operator=(glm::mat4x4&& scalar) {
	if (_datatype != Unknown) {
		throw std::logic_error("Invalid Operation");
	}
	_datatype = Mat4;
	PushItem<glm::mat4x4>(scalar);
	return *this;
}

void Datum::PushItem(Scope* item) {
	if (isExtern) {
		throw std::logic_error("Invalid Operation");
	}
	if (_datatype == Table || _datatype == Unknown) {
		_datatype = Table;
		if(Size == 0|| Size ==Capacity){
			Reserve(Size + 1);
			_data.scopeData[Size] = item;
			++Size;
		}
		else if(Size<Capacity) {
			_data.scopeData[Size] = item;
			++Size;
		}
	}
	else {
		throw std::logic_error("Type Mismatch");
	}
}


void Datum::RemoveAt(size_t idx) {
	if (_datatype == Unknown) {
		throw std::logic_error("Unknown type and has no data");
	}
	if (idx >= Size || idx < 0) {
		throw std::out_of_range("Out of range");
	}
	else {
		switch (_datatype) {
		case Int: 
			for (size_t i = idx; i < Size-1; ++i) {
				_data.intData[i] = _data.intData[i + 1];
			}
			--Size;
			break;
		case Float:
			for (size_t i = idx; i < Size - 1; ++i) {
				_data.floatData[i] = _data.floatData[i + 1];
			}
			--Size;
			break;
		case String:
			for (size_t i = idx; i < Size - 1; ++i) {
				_data.stringData[i] = _data.stringData[i + 1];
			}
			--Size;
			break;
		case Table:
			for (size_t i = idx; i < Size - 1; ++i) {
				_data.scopeData[i] = _data.scopeData[i + 1];
			}
			if (Size == 1) {
				_data.scopeData[0] = nullptr;
			}
			--Size;
			break;
		case Vec4:
			for (size_t i = idx; i < Size - 1; ++i) {
				_data.vecData[i] = _data.vecData[i + 1];
			}
			--Size;
			break;
		case Mat4:
			for (size_t i = idx; i < Size - 1; ++i) {
				_data.matData[i] = _data.matData[i + 1];
			}
			--Size;
			break;
		}
	}
}