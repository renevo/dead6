The layout of structures and classes are not written in stone; however, all member functions and variables should have the correct access permission. Member variables should be defined before member functions at the beginning. Grouping functions and variables of the same access permission is encouraged, but is not required. All commenting standards (described [here](TechDoc_CodingStandards_Commenting.md)) must be obeyed.

## Structures ##

Structures use the 't' prefix when named. Structures are only allowed to retain public access; they cannot declare functions or variables as private or protected. Structures may inherit from other classes/structures at any permission.

An example of a structure declaration:
```
struct tMyStruct
{
	// Some value...
	int m_nSomeValue;

	//////////////////////////////////////////////////////
	// SomeFunc
	//
	// Purpose: Perform some operation
	//////////////////////////////////////////////////////
	void SomeFunc(void);
};
```

## Classes ##

Classes use the 'C' prefix when named. Similarly, an interface object should use the prefix 'I'. Classes may inherit from other classes/structures at any permission.

If the class retains dynamic memory which it is responsible for allocating and freeing, both the Copy Constructor and Assignment Operator must be defined to handle this virtual memory correctly during instance copying. The virtual memory should either be duplicated with an additional alloc, or the Copy Constructor and Assignment Operator should be declared with private/protected permission.

The Destructor should always be declared as virtual if the class is inherited or makes use of inheritance.

An example of a class declaration:
```
class CMyClass
{
	// Some value...
	int m_nSomeValue;

public:
	//////////////////////////////////////////////////////
	// Constructor
	//////////////////////////////////////////////////////
	CMyClass(void);
	CMyClass(CMyClass const& obj);
	CMyClass& operator =(CMyClass const& obj);

	//////////////////////////////////////////////////////
	// Destructor
	//////////////////////////////////////////////////////
	virtual ~CMyClass(void);

	//////////////////////////////////////////////////////
	// SomeFunc
	//
	// Purpose: Perform some operation
	//////////////////////////////////////////////////////
	void SomeFunc(void);
};
```

[Back](TechDoc_CodingStandards.md)

