#pragma once

class OO{
public:
	virtual void release() = 0;
	virtual void retain() = 0;
};

template<class T>  
class Pointer:public OO{  
	class _counter{  
		template<class T> friend class Pointer;  
		_counter(int u):use(u){}  
		~_counter(){}  
		int use;  
	};  
    public:  
        Pointer(T *t):pc(new _counter(1)){  
            this->pt = t;  
        }  
  
        Pointer(const Pointer<T> &rhs){  
            this->pc = rhs.pc;  
            this->pt = rhs.pt;  
            retain();
             
        }  
		void release()
		{
			 pc->use--;  
            if(pc->use == 0)  
            {  
                delete pt;  
                delete pc;  
            }  
		}
        ~Pointer(){  
           release();
        }  
		void retain()
		{
			this->pc->use++; 
		}
        Pointer<T>& operator=(const Pointer<T> rhs){  
            this->pt = rhs.pt;  
            this->pc = rhs.pc;  
            retain();
            return *this;  
        }  
		T& operator *(){ return *pt; }  
        T* operator ->() { return pt; }  

		T* pointer()
		{
			return pt;
		}
    private:  
        T *pt;  
        _counter* pc;  
};  