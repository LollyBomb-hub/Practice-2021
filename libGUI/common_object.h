#ifndef _COMMON_OBJECT_H_

#define _COMMON_OBJECT_H_

template <typename T>
class CommonObject
{
	public:
		CommonObject(T initialValue)
		{
			this->value = initialValue;
			return;
		}
		void setValue(T newValue)
		{
			this->value = newValue;
			return;
		}
		T getValue()
		{
			return this->value;
		}
		CommonObject<T>& operator=(const T newValue)
		{
			this->setValue(newValue);
			return *this;
		}
		CommonObject<T>& operator=(const CommonObject<T> newValue)
		{
			*this = newValue;
			return *this;
		}
	private:
		T value;
};


#endif
