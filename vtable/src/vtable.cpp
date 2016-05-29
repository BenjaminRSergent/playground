#include <iostream>
bool globalChild;
enum FunctionType {
	PRINTA = 0,
	PRINTB = 1,
	OTHER = 3
};

FunctionType gobalFuncType;

class BaseA {
public:
	virtual void printA() {
		std::cout << "Inside " << __func__ << " this is " << ((!globalChild && gobalFuncType == PRINTA) ? "Correct" : "Incorrect");
	}
};

class BaseB {
public:
	virtual void printB() {
		std::cout << "Inside " << __func__  <<" this is " << ((!globalChild && gobalFuncType == PRINTB) ? "Correct" : "Incorrect");
	}
};


class DoubleParent : public BaseA, public BaseB {
public:
	virtual void printChild() {
		std::cout << "Inside " << __func__ << " this is " << ((globalChild && gobalFuncType == OTHER) ? "Correct" : "Incorrect");
	}

	virtual void printA() override {
		std::cout << "Inside " << __func__ << " this is " << ((globalChild && gobalFuncType == OTHER) ? "Correct" : "Incorrect");
	}

	virtual void printB() override{
		std::cout << "Inside " << __func__ << " this is " << ((globalChild && gobalFuncType == OTHER) ? "Correct" : "Incorrect");
	}

	virtual void printChildTwo() {
		std::cout << "Inside " << __func__ << " this is " << ((globalChild && gobalFuncType == OTHER) ? "Correct" : "Incorrect");
	}

};

typedef void (*testFunc)(void); 
void callVoidOnVtable(uintptr_t* vtable, uintptr_t obj, int numFuncs, bool isChild) {
	uintptr_t tableLoc = reinterpret_cast<uintptr_t>(vtable);
	std::cout << "The object is located at " << std::hex << obj << "\n";
	std::cout << "The Vtable is located at " << std::hex << tableLoc << "\n";

	globalChild = isChild;
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

	BaseA* actualA = new BaseA();
	BaseB* actualB = new BaseB();

	uintptr_t* doubleVtable = *(uintptr_t**)doubleParent;
	uintptr_t* secondDoubleVtable = *(uintptr_t**)secondDoubleParent;
	uintptr_t* baseAVtable = *(uintptr_t**)actualA;
	uintptr_t* baseBVtable = *(uintptr_t**)actualB;


	uintptr_t* staticCastBaseAVtable = *(uintptr_t**)staticDoubleAsA;
	uintptr_t* staticCastBaseBVtable = *(uintptr_t**)staticDoubleAsB;

	uintptr_t* reinterpretCastBaseAVtable = *(uintptr_t**)reinterpretDoubleAsA;
	uintptr_t* reinterpretCastBaseBVtable = *(uintptr_t**)reinterpretDoubleAsB;


	gobalFuncType = PRINTA;
	std::cout << "Actual Base A vtable\n";
	callVoidOnVtable(baseAVtable, reinterpret_cast<uintptr_t>(actualA), 1, false);

	gobalFuncType = PRINTB;
	std::cout << "============================\n";
	std::cout << "Actual Base B vtable\n";
	callVoidOnVtable(baseBVtable, reinterpret_cast<uintptr_t>(actualB), 1, false);

	gobalFuncType = PRINTA;
	std::cout << "============================\n";
	std::cout << "static_cast DoubleParent to BaseA vtable\n";
	callVoidOnVtable(staticCastBaseAVtable, reinterpret_cast<uintptr_t>(staticDoubleAsA), 1, true);

	gobalFuncType = PRINTB;
	std::cout << "============================\n";
	std::cout << "static_cast DoubleParent to BaseB vtable\n";
	callVoidOnVtable(staticCastBaseBVtable, reinterpret_cast<uintptr_t>(staticDoubleAsB), 1, true);

	gobalFuncType = PRINTA;
	std::cout << "============================\n";
	std::cout << "reinterpret_cast DoubleParent to BaseA vtable\n";
	callVoidOnVtable(reinterpretCastBaseAVtable, reinterpret_cast<uintptr_t>(reinterpretDoubleAsA), 1, true);

	gobalFuncType = PRINTB;
	std::cout << "============================\n";
	std::cout << "reinterpret_cast DoubleParent to BaseB vtable\n";
	callVoidOnVtable(reinterpretCastBaseBVtable, reinterpret_cast<uintptr_t>(reinterpretDoubleAsB), 1, true);

	gobalFuncType = OTHER;
	std::cout << "============================\n";
	std::cout << "First DoubleParent vtable\n";
	callVoidOnVtable(doubleVtable, reinterpret_cast<uintptr_t>(doubleParent), 4, true);

	gobalFuncType = OTHER;
	std::cout << "============================\n";
	std::cout << "Second DoubleParent vtable\n";
	callVoidOnVtable(secondDoubleVtable, reinterpret_cast<uintptr_t>(secondDoubleParent), 4, true);

	return 0;
}