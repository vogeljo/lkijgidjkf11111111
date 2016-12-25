#pragma once
#include <set>
#include <vector>

const char* _empty_exception_text = "UniqueStack is empty.";

template<typename T>
class UniqueStack
{
private:
	std::set<T> mSet;
	std::vector<T> mStack;

	using ForwardIterator = typename std::vector<T>::iterator;

	ForwardIterator Find(T value) {
		for (auto it = mStack.begin(); it != mStack.end(); ++it) {
			if (*it == value)
				return it;
		}
		return mStack.end();
	}

	ForwardIterator GetTopIterator() {
		if (!this->IsEmpty()) {
			ForwardIterator stack_it = mStack.end();
			--stack_it;
			return stack_it;
		}
		else
			throw std::exception(_empty_exception_text);
	}
public:
	UniqueStack() {

	}

	~UniqueStack() {

	}

	bool Push(T value) {
		if (!this->Contains(value)) {
			mSet.insert(value);
			mStack.push_back(value);
			return true;
		}
		return false;
	}

	bool PushSubst(T value) {
		if (this->Push(value)) {
			return true;
		}
		else {
			auto stack_it = this->Find(value);
			mStack.erase(stack_it);
			mSet.erase(value);
			return this->Push(value);
		}
	}

	T Pop() {
		if (!this->IsEmpty()) {
			auto stack_it = this->GetTopIterator();
			auto value = *stack_it;
			mSet.erase(value);
			mStack.erase(stack_it);
			return value;
		}
		else
			throw std::exception(_empty_exception_text);
	}

	T Top() {
		if (!this->IsEmpty()) {
			return mStack.top();
		}
		else
			throw std::exception(_empty_exception_text);
	}

	bool Contains(T value) {
		return mSet.find(value) != mSet.end();
	}

	bool IsEmpty() {
		return mSet.empty();
	}
};

