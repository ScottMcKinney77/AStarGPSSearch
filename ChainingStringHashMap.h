#import <string>
#import <math.h>

using namespace std;

/* header file which contains template class ChainingHashMap
*	takes a string key and a value of type T and hashes into hashmap
*	hash value of string is the product of their first and last letters (only first if length 1) mod smallest prime greater than max table size
*	uses chaining to deal with collisions
*/

template <typename T>
class HashBucket{
public:
	T* current;
	HashBucket<T>* next;
	HashBucket(){
		current = NULL;
		next = NULL;
	}
};

template <typename T>
class ChainingHashMap{
protected:
	int modValue;
	HashBucket<T>** arr;
public:
	//function which finds the smallest prime greater than n
	bool checkPrime(int n){
		int sqrd = sqrt(n);
		for (int i = 2; i <= sqrd; i++){
			if ((n % i) == 0){
				return false;
			}
		}
		return true;
	}
	int nextPrime(int n){
		while(true){
			if (checkPrime(n)){
				return n;
			}
			n++;
		}
	}
	ChainingHashMap(int maxSize){
		this->modValue = nextPrime(maxSize);
		this->arr = new HashBucket<T>*[modValue];
		for (int i = 0; i < modValue; i++){
			arr[i] = new HashBucket<T>();
		}
	}
	int getSize(){
		return this->modValue;
	}
	void add(string key, T* value){
		int location = getHash(key);
		if (this->arr[location]->current == NULL){
			this->arr[location]->current = value;
			this->arr[location]->next = new HashBucket<T>();
		}else{
			HashBucket<T>* temp = this->arr[location];
			while (temp->current != NULL){
				temp = temp->next;
			}
			temp->current = value;
			temp->next = new HashBucket<T>();
		}
	}
	T* get(string key){
		int location = getHash(key);
		if(this->arr[location]->current == NULL){
			return NULL;
		}
		HashBucket<T>* temp = this->arr[location];
		while (temp->current != NULL) {
			if (temp->current->getName() == key){
				return temp->current;
			}
			temp = temp->next;
		}
		return NULL;
	}
	int getHash(string key){
		int pkey = 35;
		if (key.length() == 1){
			pkey = (pkey*key[0]) % modValue;
			return pkey;
		}
		for (int i = 0; i < key.length(); i++){
			pkey = (key[i]*pkey) % modValue;
		}
		return pkey;
	}
};