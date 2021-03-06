#include <iostream>
bool globalChild;
enum FunctionType {
	PRINTA = 0,
	PRINTB = 1,
};

FunctionType gobalFuncType;

class BaseA {
public:
	virtual void printA() {
		std::cout << "Inside " << __func__ << " parent this is " << ((!globalChild && gobalFuncType == PRINTA) ? "Correct" : "Incorrect");
	}
};

class BaseB {
public:
	virtual void printB() {
		std::cout << "Inside " << __func__  <<" parent this is " << ((!globalChild && gobalFuncType == PRINTB) ? "Correct" : "Incorrect");
	}
};


class DoubleParent : public BaseA, public BaseB {
public:
	virtual void printChild() {
		std::cout << "Inside " << __func__ << " this is " << ((globalChild) ? "Correct" : "Incorrect");
	}

	virtual void printA() override {
		std::cout << "Inside " << __func__ << " child this is " << ((globalChild && gobalFuncType == PRINTA) ? "Correct" : "Incorrect");
	}

	virtual void printB() override{
		std::cout << "Inside " << __func__ << " child this is " << ((globalChild && gobalFuncType == PRINTB) ? "Correct" : "Incorrect");
	}

	virtual void printChildTwo() {
		std::cout << "Inside " << __func__ << " this is " << ((globalChild) ? "Correct" : "Incorrect");
	}

};

typedef void (*testFunc)(void); 
void callVoidOnVtable(uintptr_t obj, int numFuncs) {
	uintptr_t* vtable = *(uintptr_t**)obj;
	uintptr_t tableLoc = reinterpret_cast<uintptr_t>(vtable);
	std::cout << "The object is located at " << std::hex << obj << "\n";
	std::cout << "The Vtable is located at " << std::hex << tableLoc << "\n";

	for(int index = 0; index < numFuncs; index++) {
		std::cout << "Calling the function at " << std::hex << vtable[index] << " which is index " << index << "\n";
		testFunc func = reinterpret_cast<testFunc>(vtable[index]);
		func();
		std::cout << "\n";
	}
	std::cout << "\n\n";

}

int main(int argc, char** argv) {
	DoubleParent* doubleParent = new DoubleParent();
	DoubleParent* secondDoubleParent = new DoubleParent();

	BaseA* staticDoubleAsA = static_cast<BaseA*>(doubleParent);
	BaseB* staticDoubleAsB = static_cast<BaseB*>(doubleParent);
	BaseA* reinterpretDoubleAsA = reinterpret_cast<BaseA*>(doubleParent);
	BaseB* reinterpretDoubleAsB = reinterpret_cast<BaseB*>(doubleParent);

	void* voidDouble = static_cast<void*>(doubleParent);

	BaseA* voidAndBackA = static_cast<BaseA*>(voidDouble);
	BaseB* voidAndBackB = static_cast<BaseB*>(voidDouble);


	BaseA* reintVoidAndBackA = reinterpret_cast<BaseA*>(reinterpret_cast<void*>(doubleParent));
	BaseB* reintVoidAndBackB = reinterpret_cast<BaseB*>(reinterpret_cast<void*>(doubleParent));

	BaseA* actualA = new BaseA();
	BaseB* actualB = new BaseB();


	gobalFuncType = PRINTA;
	globalChild = false;
	std::cout << "Actual Base A vtable\n";
	callVoidOnVtable(reinterpret_cast<uintptr_t>(actualA), 1);

	gobalFuncType = PRINTB;
	std::cout << "============================\n";
	std::cout << "Actual Base B vtable\n";
	callVoidOnVtable(reinterpret_cast<uintptr_t>(actualB), 1);


	globalChild = true;
	gobalFuncType = PRINTA;
	std::cout << "============================\n";
	std::cout << "static_cast DoubleParent to BaseA vtable\n";
	callVoidOnVtable(reinterpret_cast<uintptr_t>(staticDoubleAsA), 1);

	gobalFuncType = PRINTB;
	std::cout << "============================\n";
	std::cout << "static_cast DoubleParent to BaseB vtable\n";
	callVoidOnVtable(reinterpret_cast<uintptr_t>(staticDoubleAsB), 1);

	gobalFuncType = PRINTA;
	std::cout << "============================\n";
	std::cout << "reinterpret_cast DoubleParent to BaseA vtable\n";
	callVoidOnVtable(reinterpret_cast<uintptr_t>(reinterpretDoubleAsA), 1);

	gobalFuncType = PRINTB;
	std::cout << "============================\n";
	std::cout << "reinterpret_cast DoubleParent to BaseB vtable\n";
	callVoidOnVtable(reinterpret_cast<uintptr_t>(reinterpretDoubleAsB), 1);

	gobalFuncType = PRINTA;
	std::cout << "============================\n";
	std::cout << "static_cast void and back DoubleParent to BaseA vtable\n";
	callVoidOnVtable(reinterpret_cast<uintptr_t>(voidAndBackA), 1);

	gobalFuncType = PRINTB;
	std::cout << "============================\n";
	std::cout << "static_cast void and back DoubleParent to BaseB vtable\n";
	callVoidOnVtable(reinterpret_cast<uintptr_t>(voidAndBackB), 1);

	gobalFuncType = PRINTA;
	std::cout << "============================\n";
	std::cout << "reinterpret_cast void and back DoubleParent to BaseA vtable\n";
	callVoidOnVtable(reinterpret_cast<uintptr_t>(reintVoidAndBackA), 1);

	gobalFuncType = PRINTB;
	std::cout << "============================\n";
	std::cout << "reinterpret_cast void and back DoubleParent to BaseB vtable\n";
	callVoidOnVtable(reinterpret_cast<uintptr_t>(reintVoidAndBackB), 1);

/*
	gobalFuncType = OTHER;
	std::cout << "============================\n";
	std::cout << "First DoubleParent vtable\n";
	callVoidOnVtable(reinterpret_cast<uintptr_t>(doubleParent), 4);

	gobalFuncType = OTHER;
	std::cout << "============================\n";
	std::cout << "Second DoubleParent vtable\n";
	callVoidOnVtable(reinterpret_cast<uintptr_t>(secondDoubleParent), 4);
*/
	return 0;
}