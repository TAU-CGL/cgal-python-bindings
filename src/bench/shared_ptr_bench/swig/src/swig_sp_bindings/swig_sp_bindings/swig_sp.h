#include <memory>
#include <random>
using namespace std;

default_random_engine generator;
//uniform_int_distribution <int> distribution(1, 10);

class MyClass
{
private:
	int val = 5; //= distribution(generator);
public:
	int get() const { return val; }
	void set(int i) { val = i; }
};


shared_ptr<MyClass> getSharedPointerToClass() {
	shared_ptr<MyClass> sp = make_shared<MyClass>();
	return sp;
}

shared_ptr<MyClass> getAnotherReference(shared_ptr<MyClass> p) {
	return p;
}

int getReferenceCount(shared_ptr<MyClass> p)
{
	return p.use_count();
}