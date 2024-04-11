#pragma once
namespace Fiea::GameEngine {
	typedef Fiea::GameEngine::Datum Datum;
	typedef std::string StdString;
	//Templatized pushback function. Checks for
	template<typename T>
	void Datum::PushItem(T& item) {
		if (isExtern) {
			throw std::logic_error("Memory is external");
		}
		if constexpr (std::is_same<T, int>::value) {
			if (_datatype != DatumType::Int && _datatype != DatumType::Unknown)
				throw std::logic_error("Type mismatch");
			if (Capacity == 0 || _datatype == DatumType::Unknown) {
				_datatype = DatumType::Int;
				_data.intData = (int*)malloc(sizeof(int));
				Size = 1;
				Capacity = 1;
				_data.intData[Size - 1] = item;
				return;
			}
			if (Size < Capacity) {
				new(_data.intData + Size) int(item);
				++Size;
			}
			else {
				Capacity *= 2;
				Reserve(Capacity * 2);
				new(_data.intData + Size) int(item);
				++Size;
			}
		}
		else if constexpr (std::is_same<T, float>::value) {
			if (_datatype != DatumType::Float && _datatype != DatumType::Unknown)
				throw std::logic_error("Type mismatch");
			if (Capacity == 0 || _datatype == DatumType::Unknown) {
				_datatype = DatumType::Float;
				_data.floatData = (float*)malloc(sizeof(float));
				Size = 1;
				Capacity = 1;
				_data.floatData[Size - 1] = item;
				return;

			}
			if (Size < Capacity) {
				new(_data.floatData + Size) float(item);
				++Size;
			}
			else {
				Capacity *= 2;
				Reserve(Capacity * 2);
				new(_data.floatData + Size) float(item);
				++Size;
			}
		}
		else if constexpr (std::is_same < T, std::string > ::value) {
			if (_datatype != DatumType::String && _datatype != DatumType::Unknown)
				throw std::logic_error("Type mismatch");
			if (Capacity == 0 || _datatype == DatumType::Unknown) {
				_datatype = DatumType::String;
				_data.stringData = (StdString*)malloc(sizeof(StdString));
				Size = 1;
				Capacity = 1;
				new(_data.stringData) std::string(item);
				return;

			}
			if (Size < Capacity) {
				new(_data.stringData + Size) std::string(item);
				++Size;
				return;
			}
			else {
				Capacity *= 2;
				Reserve(Capacity * 2);
				new(_data.stringData + Size) std::string(item);
				++Size;
				return;
			}
		}
		else if constexpr (std::is_same<T, glm::vec4>::value) {
			if (_datatype != DatumType::Vec4 && _datatype != DatumType::Unknown)
				throw std::logic_error("Type mismatch");
			if (Capacity == 0 || _datatype == DatumType::Unknown) {
				_datatype = DatumType::Vec4;
				_data.vecData = (glm::vec4*)malloc(sizeof(glm::vec4));
				Size = 1;
				Capacity = 1;
				new(_data.vecData) glm::vec4(item);
				return;

			}
			if (Size < Capacity) {
				new(_data.vecData + Size) glm::vec4(item);
				++Size;
				return;
			}
			else {
				Capacity *= 2;
				Reserve(Capacity * 2);
				new(_data.vecData + Size) glm::vec4(item);
				++Size;
				return;
			}
		}
		else if constexpr (std::is_same<T, glm::mat4x4>::value) {
			if (_datatype != DatumType::Mat4 && _datatype != DatumType::Unknown)
				throw std::logic_error("Type mismatch");
			if (Capacity == 0 || _datatype == DatumType::Unknown) {
				_datatype = DatumType::Mat4;
				_data.matData = (glm::mat4x4*)malloc(sizeof(glm::mat4x4));
				Size = 1;
				Capacity = 1;
				new(_data.matData) glm::mat4x4(item);
				return;
			}
			if (Size < Capacity) {
				new(_data.matData + Size) glm::mat4x4(item);
				++Size;
				return;
			}
			else {
				Capacity *= 2;
				Reserve(Capacity * 2);
				new(_data.matData + Size) glm::mat4x4(item);
				++Size;
				return;
			}
		}
	}
	template<typename T>
	T& Datum::GetItemAsValue(size_t idx) const {
		if (idx < 0 || idx >= Size)
			throw std::out_of_range("Out of range");
		if constexpr (std::is_same<T, int>::value) {
			if (_datatype != DatumType::Int)
				throw std::logic_error("Type mismatch");
			return _data.intData[idx];
		}
		else if constexpr (std::is_same<T, float>::value) {
			if (_datatype != DatumType::Float)
				throw std::logic_error("Type mismatch");
			if (idx < 0 || idx >= Size)
				throw std:: out_of_range("Out of range");
			return _data.floatData[idx];
		}
		else if constexpr (std::is_same<T, std::string>::value) {
			if (_datatype != DatumType::String)
				throw std::logic_error("Type mismatch");
			if (idx < 0 || idx >= Size)
				throw std::out_of_range("Out of range");
			return _data.stringData[idx];
		}
		else if constexpr (std::is_same<T, glm::vec4>::value) {
			if (_datatype != DatumType::Vec4)
				throw std::logic_error("Type mismatch");
			if (idx < 0 || idx >= Size)
				throw std::out_of_range("Out of range");
			return _data.vecData[idx];
		}
		else if constexpr (std::is_same<T, glm::mat4x4>::value) {
			if (_datatype != DatumType::Mat4)
				throw std::logic_error("Type mismatch");
			if (idx < 0 || idx >= Size)
				throw std::out_of_range("Out of range");
			return _data.matData[idx];
		}
		else if constexpr (std::is_same<T, Scope*>::value) {
			if (_datatype != DatumType::Table)
				throw std::logic_error("Type mismatch");
			if (idx < 0 || idx >= Size)
				throw std::out_of_range("Out of range");
			return _data.scopeData[idx];
		}
	}

	template<typename T>
	void Datum::SetItemWithValue(T& value, size_t idx) {
		if (idx >= Size || idx < 0) {
			throw std::out_of_range("Index out of range");
		}
		else {
			if constexpr (std::is_same<T, int>::value) {
				if (_datatype != DatumType::Int)
					throw std::logic_error("Type mismatch");
				_data.intData[idx] = value;
			}
			else if constexpr (std::is_same<T, float>::value) {
				if (_datatype != DatumType::Float)
					throw std::logic_error("Type mismatch");
				_data.floatData[idx] = value;
			}
			else if constexpr (std::is_same<T, StdString>::value) {
				if (_datatype != DatumType::String)
					throw std::logic_error("Type mismatch");
				_data.stringData[idx] = value;
			}
			else if constexpr (std::is_same<T, glm::vec4>::value) {
				if (_datatype != DatumType::Vec4)
					throw std::logic_error("Type mismatch");
				_data.vecData[idx] = value;
			}
			else if constexpr (std::is_same<T, glm::mat4x4>::value) {
				if (_datatype != DatumType::Mat4)
					throw std::logic_error("Type mismatch");
				_data.matData[idx] = value;
			}
		}
	}

	template<typename T>
	void Datum::SetItemWithValue(T&& value, size_t idx) {
		if (idx >= Size || idx < 0) {
			throw std::out_of_range("Index out of range");
		}
		else {
			if constexpr (std::is_same<T, int>::value) {
				if (_datatype != DatumType::Int)
					throw std::logic_error("Type mismatch");
				_data.intData[idx] = value;
			}
			else if constexpr (std::is_same<T, float>::value) {
				if (_datatype != DatumType::Float)
					throw std::logic_error("Type mismatch");
				_data.floatData[idx] = value;
			}
			else if constexpr (std::is_same<T, StdString>::value) {
				if (_datatype != DatumType::String)
					throw std::logic_error("Type mismatch");
				_data.stringData[idx] = value;
			}
			else if constexpr (std::is_same<T, glm::vec4>::value) {
				if (_datatype != DatumType::Vec4)
					throw std::logic_error("Type mismatch");
				_data.vecData[idx] = value;
			}
			else if constexpr (std::is_same<T, glm::mat4x4>::value) {
				if (_datatype != DatumType::Mat4)
					throw std::logic_error("Type mismatch");
				_data.matData[idx] = value;
			}
		}
	}

	template<typename T>
	void Datum::PushItem(T&& item) {
		if (isExtern) {
			throw std::logic_error("Memory is external");
		}
		if constexpr (std::is_same<T, int>::value) {
			if (_datatype != DatumType::Int && _datatype != DatumType::Unknown)
				throw std::logic_error("Type mismatch");
			if (Capacity == 0 || _datatype == DatumType::Unknown) {
				_datatype = DatumType::Int;
				_data.intData = (int*)malloc(sizeof(int));
				Size = 1;
				Capacity = 1;
				_data.intData[Size - 1] = item;
				return;
			}
			if (Size < Capacity) {
				new(_data.intData + Size) int(item);
				++Size;
			}
			else {
				Capacity *= 2;
				Reserve(Capacity * 2);
				new(_data.intData + Size) int(item);
				++Size;
			}
		}
		else if constexpr (std::is_same<T, float>::value) {
			if (_datatype != DatumType::Float && _datatype != DatumType::Unknown)
				throw std::logic_error("Type mismatch");
			if (Capacity == 0 || _datatype == DatumType::Unknown) {
				_datatype = DatumType::Float;
				_data.floatData = (float*)malloc(sizeof(float));
				Size = 1;
				Capacity = 1;
				_data.floatData[Size - 1] = item;
				return;

			}
			if (Size < Capacity) {
				new(_data.floatData + Size) float(item);
				++Size;
			}
			else {
				Capacity *= 2;
				Reserve(Capacity * 2);
				new(_data.floatData + Size) float(item);
				++Size;
			}
		}
		else if constexpr (std::is_same < T, std::string > ::value) {
			if (_datatype != DatumType::String && _datatype != DatumType::Unknown)
				throw std::logic_error("Type mismatch");
			if (Capacity == 0 || _datatype == DatumType::Unknown) {
				_datatype = DatumType::String;
				_data.stringData = (StdString*)malloc(sizeof(StdString));
				Size = 1;
				Capacity = 1;
				new(_data.stringData) std::string(item);
				return;

			}
			if (Size < Capacity) {
				new(_data.stringData + Size) std::string(item);
				++Size;
				return;
			}
			else {
				Capacity *= 2;
				Reserve(Capacity * 2);
				new(_data.stringData + Size) std::string(item);
				++Size;
				return;
			}
		}
		else if constexpr (std::is_same<T, glm::vec4>::value) {
			if (_datatype != DatumType::Vec4 && _datatype != DatumType::Unknown)
				throw std::logic_error("Type mismatch");
			if (Capacity == 0 || _datatype == DatumType::Unknown) {
				_datatype = DatumType::Vec4;
				_data.vecData = (glm::vec4*)malloc(sizeof(float));
				Size = 1;
				Capacity = 1;
				new(_data.vecData) glm::vec4(item);
				return;

			}
			if (Size < Capacity) {
				new(_data.vecData + Size) glm::vec4(item);
				++Size;
				return;
			}
			else {
				Capacity *= 2;
				Reserve(Capacity * 2);
				new(_data.vecData + Size) glm::vec4(item);
				++Size;
				return;
			}
		}
		else if constexpr (std::is_same<T, glm::mat4x4>::value) {
			if (_datatype != DatumType::Mat4 && _datatype != DatumType::Unknown)
				throw std::logic_error("Type mismatch");
			if (Capacity == 0 || _datatype == DatumType::Unknown) {
				_datatype = DatumType::Mat4;
				_data.matData = (glm::mat4x4*)malloc(sizeof(glm::mat4x4));
				Size = 1;
				Capacity = 1;
				new(_data.matData) glm::mat4x4(item);
				return;
			}
			if (Size < Capacity) {
				new(_data.matData + Size) glm::mat4x4(item);
				++Size;
				return;
			}
			else {
				Capacity *= 2;
				Reserve(Capacity * 2);
				new(_data.matData + Size) glm::mat4x4(item);
				++Size;
				return;
			}
		}
	}


	template<typename T>
	void Datum::SetStorage(T* Extern, size_t size) {
		if (_datatype != Unknown) {
			clear();
		}
		if constexpr (std::is_same<T, int>::value) {
			_datatype = Int;
			Size = size;
			Capacity = size;
			_data.intData = Extern;
		}
		else if constexpr (std::is_same<T, float>::value) {
			_datatype = Float;
			Size = size;
			Capacity = size;
			_data.floatData = Extern;
		}
		else if constexpr (std::is_same<T, std::string>::value) {
			_datatype = String;
			Size = size;
			Capacity = size;
			_data.stringData = Extern;
		}
		else if constexpr (std::is_same<T, glm::vec4>::value) {
			_datatype = Vec4;
			Size = size;
			Capacity = size;
			_data.vecData = Extern;
		}
		else if constexpr (std::is_same<T, glm::mat4x4>::value) {
			_datatype = Mat4;
			Size = size;
			Capacity = size;
			_data.matData = Extern;
		}
		else if constexpr (std::is_same<T, Scope*>::value) {
			_datatype = Table;
			Size = size;
			Capacity = size;
			_data.scopeData = Extern;
		}
		else if constexpr (std::is_same<T, RTTI*>::value) {
			_datatype = Pointer;
			Size = size;
			Capacity = size;
			_data.pointerData = Extern;
		}
		isExtern = true;
	}

}