/*
Name:				Deron Washington II (DGOOOOOOOOOOOOOOOAT)
Class:				CECS 282
Instructor :			Minhthong Nguyen
Purpose:			Complex Number Calculator
Last updated :	11/11/2019
*/

#include "pch.h"
#include "nodeType.h"
#include "DoublyLinkedList.h"
#include "StackDoublyLinkedList.h"
#include "Complex.h"
#include "myFunctions.h"


bool isLeftParen(const char c)
{
	if (c == '(')
		return true;

	return false;
}

bool isRightParen(const char c)
{
	if (c == ')')
		return true;

	return false;
}

bool isParen(const char c)
{
	if (c == '(' || c == ')')
		return true;

	return false;
}

//function to see if the character taken in is an operator
bool isOperator(const char c)
{
	// check for the various operators
	if ((c == '+') || (c == '-')
		|| (c == '*') || (c == '/')
		|| (c == '%'))
	{
		return true; // return true if it is
	}

	else
	{
		return false; //false if character is not an operator
	}

} // end function isOperator

int precedence(char op1, char op2)
//determines the precedence between two operators
//Postcondition: returns value true if the first operator is of higher precedence
//than the second operator returns the value false
{
	int pre1,
		pre2; //declare variables to compare the precedence

	//GETS PRECEDENCE OF THE FIRST OPERATOR
	if ((op1 == '+') || (op1 == '-'))
	{
		pre1 = 1;  // high precedence
	}
	else if ((op1 == '*') || (op1 == '/'))
	{
		pre1 = 0; // low precedence
	}
	else if (op1 == '%')
		pre1 = 1; // high precedence
	/*else if (isRightParen(op1) == true)
		pre1 = 100;*/
	else
		pre1 = -1;

	//GETS PRECEDENCE OF THE SECOND OPERATOR
	if ((op2 == '+') || (op2 == '-'))
	{
		pre2 = 1;  // high precedence
	}
	else if ((op2 == '*') || (op2 == '/'))
	{
		pre2 = 0; // low precedence
	}
	else if (op2 == '%')
		pre2 = 1; // high precedence
	/*else if (isRightParen(op2) == true)
		pre2 = 100;*/
	else
		pre2 = -1;

	//// if left paren keep it same place so make them equal precedence
	//if (isLeftParen(op1) == true
	//	|| isLeftParen(op2) == true)
	//	pre2 = pre1;

	//if (isRight)

	//COMPARE THE PRECEDENCE OF THE OPERATORS AND RETURN TRUE OR FALSE

	if (pre1 == pre2) //equal precedence of op1 & op2 return 0;
	{
		return 0;
	}
	else if (pre1 > pre2) //higher precedence of op1 over op2 return 1;
	{
		return 1;
	}
	else //lower precedence of op1 over op2
	{
		return -1;
	}

} // end function precedence

int numOfOperands(const char c)
{
	if (c == '+'
		|| c == '-'
		|| c == '*'
		|| c == '/')
		return 2;

	else if (c == '%')
		return 1;
}

// for first equation with parentheses putting the '/' inside parentheses
// consider using a recursive function to turn to postfix until you see 
// a left paren which is where recursion would happen and stop the
// recursion when you encounter a right parentheses
//  USE THIS IF NO PARENTHESE IN ONELINE
void operatorToPostfix(StackDoublyLinkedList<char>& _operators)
{
	char op1 = '/0',
		op2 = '/0';

	int op1Deep = 0, // keeps track of how far it goes into the operators stack to get to op1
		op2Deep = 0; // keeps track of how far it goes into the operators stack to get to op2

	nodeType<char>* traverserOp1;
	nodeType<char>* traverserOp2;
	//DoublyLinkedList<char> operatorsTemp = _operators;

	// check precedence of operators(only can swap if there are at least 2 operators)
	if (_operators.getSize() > 1)
	{
		for (DoublyLinkedListIterator<char>* it = &_operators.begin(); it->current->next != nullptr; )
		{
			// find non-parenthesis operator
			while (isParen(it->current->data) == true && it->current->next != nullptr)
			{
				++(*it);
				op1Deep++;
			}

			// store next non-parenthesis operator to compare and update iterator
			op1 = **it; // .current->data;
			traverserOp1 = it->current;//.current; // holds position of the first operator
			++(*it);
			op2Deep++;

			// if at the end of the operators stack then already postfix
			if (it->current == nullptr)
			{
				return;
			}

			// find non-parenthesis operator
			while (it->current->next != nullptr && isParen(it->current->data) == true)
			{
				++(*it);
				op2Deep++;
			}

			// if at the end of the operators stack then already postfix
			if (it->current == nullptr)
				return;

			// store next non-parenthesis operator to compare and update will happen through loop
			op2 = it->current->data;
			traverserOp2 = it->current;

			// we need to swap the two (essentially converts to postfix as you go through) 
			if (precedence(op1, op2) < 0)
			{
				//  swap only the data 
				char temp = op1;
				traverserOp1->data = op2;
				traverserOp2->data = temp;
			}

		} // end for loop
	} // end if have enough operators to switch
}

string insertParen(string _oneLine)
{
	string newString = _oneLine;
	char currOp = 0;
	char nextOp = 0;
	bool findRightParenLoc = false;

	for (int i = 0; i < newString.length() ; i++)
	{
		// only add parentheses after a '*' or '/' or '%' or '=' skip everything else
		if (newString[i] == '*' || newString[i] == '/' || newString[i] == '%' || newString[i] == '=')
		{
			// at the end
			if (newString[i] == '=' && findRightParenLoc == true)
			{
				newString.insert(i , ")");
				findRightParenLoc = false;
				break;
			}

			currOp = newString[i];
			nextOp = newString[i + 1];

			if (isLeftParen(nextOp) == false && findRightParenLoc == false)
				//{ ONLY DO IF THE NEXT OPERATOR ISN'T A '('
				// insert '(' only after '*' or '/'
			{
				if (newString[i] == '*' || newString[i] == '/')
				{
					newString.insert(i + 1, "(");
					findRightParenLoc = true;
				}
			}

			else if (findRightParenLoc == true)
			{
				// look for spot to place right parenthesis

				// encounter one of the below characters
					newString.insert(i, ")");
					findRightParenLoc = false;
			}

			/**
			place ')' when you are at the end of the list
			or when you encounter a '%', '*', or a '/'
			*/
			//}
		}
	}
	
	return newString;
}

StackDoublyLinkedList<char> operatorToPostfixParen(StackDoublyLinkedList<char>& _operators)
{
	// final version
	StackDoublyLinkedList<char> finalOp;
	nodeType<char>* curr = _operators.begin().current; // current iterator
	char op1 = 0,
		op2 = 0;
	int previousI = 0; // keeps track of the previous i to stop duplicates
	bool betweenRightParenAndLeftParen = false; // current operator is between ')' operator '('
	nodeType<char>* compare1 = nullptr;
	nodeType<char>* compare1prev = nullptr;
	char compareData = 0;
	int compareI = 0;

	int i = 0;

	/*for (int i = 0; i < _operators.getSize(); i++)*/
	while (_operators.getSize() != finalOp.getSize() && curr != nullptr)
	{
		// current char and next char is not a parenthesis and next char exists
		if (curr->next != nullptr && isParen(curr->data) == false && isParen(curr->next->data) == false)
		{
			// compare operators normally
			// if we need to swap the two (essentially converts to postfix as you go through) 
			op1 = curr->data;
			op2 = curr->next->data;

			// swap necessary?
			if (precedence(op1, op2) < 0)
			{
				// if list is empty automatically add both in swapped order
				if (finalOp.isEmptyList() == true)
				{
					//  add to finalOp in the correct order
					finalOp.addBack(op2);
					finalOp.addBack(op1);
					i++; // update i because we just worked on two characters

					// update current // ADDED
					curr->data = op2;
					curr->next->data = op1;
				}

				else // finalOp is not empty
				{
					// check to determine whether the last added char is the same char you are using to compare

					// YES (adding both will cause duplication)
					if (curr->data == finalOp.end().current->data
						&& previousI + 1 == i)
					{
						// only add op2 
						finalOp.addBack(op2);

						// swap the last two operators
						char temp = finalOp.end().current->prev->data;
						finalOp.end().current->prev->data = finalOp.end().current->data;
						finalOp.end().current->data = temp;

						// update current 
						temp = curr->data;
						curr->data = curr->next->data;
						curr->next->data = temp;
					}

					// NO (adding both will not cause duplication)
					else
					{
						//  add to finalOp in the correct order
						finalOp.addBack(op2);
						finalOp.addBack(op1);
						i++; // update i because we just worked on two characters
					}
				} // end of else finalOp is not empty


			} // end of if swap is necessary

			else // swap not necessary
			{
				// check to prevent duplication
				// YES (adding both will cause duplication)
				if (finalOp.end().current != nullptr
					&& curr->data == finalOp.end().current->data
					&& previousI + 1 == i)
				{
					// only add op2 
					finalOp.addBack(op2);
				}

				// NO (adding both will not cause duplication)
				else
				{
					//  add to finalOp in the correct order
					finalOp.addBack(op1);
					finalOp.addBack(op2);
					i++; // update i because we just worked on two characters
				}

				//finalOp.addBack(op1);
				//finalOp.addBack(op2);
				//i++; // update i because we just worked on two characters

			} // end swap not necessary

			previousI = i;
		}

		// need to compare for postfix notation across a set of parentheses is rightParen was compare1 != nullptr and compare1prev != nullptr curr->prev->data w no curr->next != nullptr
		else if (compare1 != nullptr /*&& compare1prev != nullptr*/ && isRightParen(curr->prev->data) == true)
		{
			op1 = compare1->data;
			op2 = curr->data;

			if (isRightParen(op2) == true) // FIX ATTEMPT
			{
				finalOp.addBack(op1);
				finalOp.addBack(curr->data);
				return finalOp;
			}

			//if (op1 == '%')
			//{
			//	// add node after compare1prev because mod must be evaluated immediately
			//	finalOp.addNode(op1, compare1prev);

			//	// add current to the back
			//	finalOp.addBack(op2);
			//} // ONLY OCCURS IF IT IS AN ERROR HERE


			// should op 1 be before op2  necessary?
			//if (precedence(op1, op2) < 0)
			if (op2 != '%')
			{
				// if list is empty means there is an error
				if (finalOp.isEmptyList() == true)
				{
					cout << endl << endl << "ERROR: Operation can not be performed. Invalid equation";
					exit(0);
				}

				else // finalOp is not empty
				{
					// check to determine whether to keep the the last added char is the same char you are using to compare

					//// YES (adding both will cause duplication)
					//if (curr->data == finalOp.end().current->data
					//	&& previousI + 1 == i)
					//{
					//	// only add op2 
					//	finalOp.addBack(op2);

					//	// swap the last two operators
					//	char temp = finalOp.end().current->prev->data;
					//	finalOp.end().current->prev->data = finalOp.end().current->data;
					//	finalOp.end().current->data = temp;
					//}

					//// NO (adding both will not cause duplication)
					//else
					//{
						//  add to finalOp in the correct order
					finalOp.addBack(op1);
					compare1 = curr; // save for later ADDED
					compare1prev = curr->prev; // save for later ADDED
					//finalOp.addBack(op2); HERE 
					//i++; // update i because we just worked on two characters HERE
				//}

				} // end of else finalOp is not empty
			} // end of if swap is necessary

			else if (op2 == '%')// swap not necessary
			{
				finalOp.addBack(op2);
				finalOp.addBack(op1);
				i++; // update i because we just worked on two characters

			// reset 
				compare1 = nullptr;
				compare1prev = nullptr;
			}

			previousI = i;

			//if (compare1prev)

			//// reset 
			//compare1 = nullptr;
		}

		// current not left paren and next char exists and is a left paren
		else if (isLeftParen(curr->data) == false && curr->next != nullptr && isLeftParen(curr->next->data) == true)
		{
			// save current node and previous for comparison
			compare1 = curr;
			compare1prev = finalOp.end().current;
			compareData = curr->data;

			if (compare1prev != nullptr && finalOp.isEmptyList() == false) // ADDED 12/3
				if (compare1->data == compare1prev->data)
				{
					// they are the same so pop 
					finalOp.popBack();
					compare1prev = finalOp.end().current;
				}

		}

		// current char is a left parenthesis
		else if (isLeftParen(curr->data) == true)
		{
			// add to finalOp
			finalOp.addBack(curr->data);
			//betweenRightParenAndLeftParen = false; // between left and right now

			if (curr->next != nullptr)
			{
				// recursive call with simpler list which consists of the rest of the list without the discovered left paren
				StackDoublyLinkedList<char> subList = (StackDoublyLinkedList<char>) _operators.subList(curr->next, _operators.end().current);
				StackDoublyLinkedList<char> postfixSubList = operatorToPostfixParen(subList);
				finalOp.join(postfixSubList); // update finalOp
				i += postfixSubList.getSize(); // update i

				// advance current as many times as postfixSubList
				for (int i = 0; i < postfixSubList.getSize(); i++)
					curr = curr->next;


				//// search rest of list for left paren
				//nodeType<char>* temp = postfixSubList.begin().current;

				//while (temp != nullptr)
				//{

				//}
			}

			else // error can't have left parentheses and the next char be nullptr
			{
				cout << endl << endl << "The operation could not be performed. Invalid notation" << endl;
				exit(0);
			}
		}

		// current char is a right parenthesis
		else if (isRightParen(curr->data) == true) // base case
		{
			// already turned to postfix so add to the finalOp
			/*DoublyLinkedList<char> subList = _operators.subList(_operators.begin().current, curr);
			finalOp.join(_operators);*/
			finalOp.addBack(curr->data);
			return finalOp;

		}

		// else if none of the above are true and curr is a non parenthetic operator
		else if (isParen(curr->data) == false && finalOp.isEmptyList() == true)
			finalOp.addBack(curr->data);

		// update statement
		if (curr != nullptr)
			curr = curr->next;
		else
			break;

		/*if (curr->next == nullptr)
			break;*/
			// ADDED 
		i++;
	} // end of for loop

	// empty compare1
	if (compare1 != nullptr)
		finalOp.addBack(compareData);


	return finalOp;
}

StackDoublyLinkedList<Complex> performOperation(StackDoublyLinkedList<Complex>& _complexNums, StackDoublyLinkedList<char>& _operators)
{
	StackDoublyLinkedList<Complex> finalResult;

	// run until all operations have been executed and operators container is empty
	while (_operators.begin().current != nullptr)
	{

		// perform the calculation
		switch (_operators.begin().current->data)
		{
		case '+':
		{

			Complex sum;

			// ADDED FOR THE LAST ONE------------------------------------------------------------
		//	if (isLeftParen(_operators.begin().current->next->data) == true)
		//	{
		//		StackDoublyLinkedList<char> subList = (StackDoublyLinkedList<char>)_operators.subList(_operators.begin().current->next, _operators.end().current);
		//		finalResult.join(performOperation(_complexNums, subList));

		//		// update operators 
		//		nodeType<char>* start = _operators.begin().current->next;
		//		nodeType<char>* matchingRightParen = nullptr;
		//		StackDoublyLinkedList<char> betweenLeftAndRightParen;
		//		nodeType<char>* nextLeftParen = nullptr;
		//		int i = 0;

		//		// find the matching right paren 
		//		while (i >= 0)
		//		{
		//			i++;

		//			// find next right paren
		//			matchingRightParen = _operators.findNext(start, ')');

		//			// didn't find right paren
		//			if (matchingRightParen == nullptr)
		//			{
		//				cout << "\n\nError no matching parentheses";
		//				exit(0);
		//			}

		//			// create SDLL between the left paren (which we r currently analyzing) and the suspected matchingRightParen
		//			betweenLeftAndRightParen = (StackDoublyLinkedList<char>) _operators.subList(_operators.begin().current->next, matchingRightParen);

		//			// attempt to find the next left paren in this new sublist
		//			nextLeftParen = betweenLeftAndRightParen.findNext(betweenLeftAndRightParen.begin().current->next, '(');

		//			// if it finds another left paren within this subList
		//			if (nextLeftParen != nullptr)
		//			{
		//				start = matchingRightParen;

		//				if (i > 25)
		//					cout << "LINE 734 INFINITE LOOP";
		//			}

		//			// no left paren within the sublist then exit loop
		//			else if (nextLeftParen == nullptr)
		//				break;
		//		}

		//		// delete all operators between the current left parentheses and it's matching right parentheses
		//		nodeType<char>* current = _operators.begin().current->next;

		//		do
		//		{
		//			_operators.deleteNode(current->data, current);
		//		} while (current != matchingRightParen);
		//	

		//	sum = _complexNums.begin().current->data + finalResult.end().current->data;

		//	// pop operands
		//	finalResult.popBack();
		//	_complexNums.popFront();

		//	// add sum to finalResult
		//	finalResult.addBack(sum);
		//}

			//  ----------------------------------------------------------------------------------------------

			// one of the operands is in the finalResult stack and the other is in the complex number stack
			if (_complexNums.getSize() == 1 || (_operators.begin().current->next->next != nullptr &&  isRightParen(_operators.begin().current->next->next->data) == true))
			{
				// one operand is from the front of the complex number stack 
				// one operand from the back of the final result stack
				sum = _complexNums.begin().current->data + finalResult.end().current->data;

				// pop the operands
				_complexNums.popFront();
				finalResult.popBack();

				// add sum to results stack
				finalResult.addBack(sum);

			}

			// no more complex numbers in the complex number stack
			else if (_complexNums.getSize() == 0)
			{
				if (finalResult.getSize() > 1)
				{
					// get last two numbers in the finalResult stack as operands
					sum = finalResult.end().current->prev->data + finalResult.end().current->data;

					// pop the operands 
					finalResult.popBack();
					finalResult.popBack();

					// add the sum to finalResult stack
					finalResult.addBack(sum);
				}

				else
				{
					// not enough operands to perform given operation
					cout << "\nCouldn't perform operation.";
					system("pause");
					exit(0);
				}
			}

			// grab first two complex numbers from complex number stack as operands
			else
			{
				// normal case
				sum = _complexNums.begin().current->data + _complexNums.begin().current->next->data;

				// pop operands
				_complexNums.popFront();
				_complexNums.popFront();

				// add sum to the finalResult stack
				finalResult.addBack(sum);
			} // end else

		} // end '+'
		break;

		case '-':
		{

			Complex difference;

			// only one number in the complex number stack
			if (_complexNums.getSize() == 1)
			{
				// one operand is from the front of the complex number stack 
				// one operand from the back of the final result stack
				difference = finalResult.end().current->data - _complexNums.begin().current->data;

				// pop the operands
				_complexNums.popFront();
				finalResult.popBack();

				// add sum to results stack
				finalResult.addBack(difference);

			}

			// no more complex numbers in the complex number stack
			else if (_complexNums.getSize() == 0)
			{
				if (finalResult.getSize() > 1)
				{
					// get last two numbers in the finalResult stack as operands
					difference = finalResult.end().current->prev->data - finalResult.end().current->data;

					// pop the operands 
					finalResult.popBack();
					finalResult.popBack();

					// add the sum to finalResult stack
					finalResult.addBack(difference);
				}

				else
				{
					// not enough operands to perform given operation
					cout << "\nCouldn't perform operation.";
					system("pause");
					exit(0);
				}
			}

			// grab first two complex numbers from complex number stack as operands
			else
			{
				// normal case
				difference = _complexNums.begin().current->data - _complexNums.begin().current->next->data;

				// pop operands
				_complexNums.popFront();
				_complexNums.popFront();

				// add sum to the finalResult stack
				finalResult.addBack(difference);
			} // end else

		} // end '-'
		break;

		case '*':
		{

			Complex product;

			if (finalResult.getSize() == 2)
			{
				// get last two numbers in the finalResult stack as operands
				product = finalResult.end().current->prev->data * finalResult.end().current->data;

				// pop the operands 
				finalResult.popBack();
				finalResult.popBack();

				// add the product to finalResult stack
				finalResult.addBack(product);
			}

			else
			{
				// not enough operands to perform given operation
				cout << "\nCouldn't perform operation.";
				system("pause");
				exit(0);
			} // end else

		} // end '*'
		break;

		case '/':
		{

			Complex quotient;

			if (finalResult.getSize() >= 2)
			{
				// get last two numbers in the finalResult stack as operands
				quotient = finalResult.end().current->prev->data / finalResult.end().current->data;

				// pop the operands 
				finalResult.popBack();
				finalResult.popBack();

				// add the product to finalResult stack
				finalResult.addBack(quotient);
			}

			else
			{
				// not enough operands to perform given operation
				cout << "\nCouldn't perform operation.";
				system("pause");
				exit(0);
			} // end else

		} // '/'
		break;

		case '%':
		{
			Complex conjugate = finalResult.end().current->data;
			conjugate.im *= -1;

			/*
			delete complex number only one operand so one time
			*/
			finalResult.popBack();

			// add node to the complex container
			finalResult.addBack(conjugate);
		} // end '%'
		break;

		case '(':
		{
			// perform operations until you find the matching ')'
			StackDoublyLinkedList<char> subList = (StackDoublyLinkedList<char>)_operators.subList(_operators.begin().current->next, _operators.end().current);
			StackDoublyLinkedList<Complex> subOperation = performOperation(_complexNums, subList);

			// update finalResult and operators
			finalResult.join(subOperation);
			_operators = subList;


		} // end left paren
		break;

		case ')':
		{
			return finalResult;
		} // end right paren
		break;

		default:
		{
			cout << "Operand doesn't exist";
			system("pause");
			exit(0); // close program
		}

		} // end switch 

		// pop operator & store as prevOperator
		_operators.popFront();

	} // end while to perform all calculations			

	return finalResult;
}

void writeToOutputFile()
{
	ofstream outFile("result.txt");

	

}

/*
Creates complex number in the correct format
@param
			oneLine    = one line of input from file
			i			   = current position in the line from input file
			tempNum = string to represent the number
@return
			= the complex number represented by tempNum in oneLine
*/
Complex createComplexNum(string oneLine, int i, string tempNum)
{
	// create complex number 
	Complex newNum;

	if (oneLine[i - 1] == 'i' || oneLine[i] == 'i') // possibly get rid of part before the or
		newNum.im = stoi(tempNum);
	else if (oneLine[i - 1] == '/')
		newNum.dem = stoi(tempNum);
	else
		newNum.re = stoi(tempNum);

	return newNum;
}

void main()
{
	// MAIN 
	ifstream inFile("expressions.txt");
	ofstream outFile("result.txt");
	string oneLine = "";
	StackDoublyLinkedList<char> operators;
	StackDoublyLinkedList<Complex> complex;
	StackDoublyLinkedList<Complex> finalResult;
	string tempNum = "";

	
	//if (inFile.good())
	//{
	//	if (inFile.is_open()) // if file is open
	//	{
	//		while (!inFile.eof()) // while not eof
	//		{
	//			// process the line and make it into a complex number
	//			//perform necessary operations then output to result.txt
	//			getline(inFile, oneLine);
	//			
	//			//  loop through oneLine until it has been fully processed
	//			for (int i = 0; i < oneLine.length(); i++)
	//			{
	//				// if character is a space or an i with digit in front
	//				if (oneLine[i] == 32
	//					|| oneLine[i] == 'i'
	//					&& isdigit(oneLine[i - 1]) == true)
	//					continue;

	//				// if character is an i with no digit in front
	//				if (oneLine[i] == 'i'
	//					&& isdigit(oneLine[i - 1]) == false)
	//					tempNum += '1';


	//				// if the character is a digit
	//				if (isdigit(oneLine[i]))
	//				{
	//					// add number to tempNum to make complex number
	//					tempNum += oneLine[i];

	//					//// push on # stack
	//					//numbers.addBack(oneLine[i]);

	//					//// push x on operator stack to keep track for equal # in both containers
	//					//operators.addBack('x');

	//					// may not need
	//					continue; // back up to update statement
	//				}

	//				//// if the character is an i
	//				//else if (oneLine[i] == 'i')
	//				//{
	//				//	// create complex number by popping all remaining chars till '(' or till stack is empty


	//				//}

	//				// if character is an operator
	//				else if (isOperator(oneLine[i]) == true
	//						|| isLeftParen(oneLine[i]) == true
	//						|| isRightParen(oneLine[i]) == true
	//					|| (tempNum == "1" && oneLine[i] == 'i') )
	//					  {
	//							// time to create complex number if more than one s
	//							if (i > 0
	//								&& oneLine[i] != '%' && tempNum != "") // so won't access array position less than 0
	//							{
	//								// create complex number 
	//								Complex newNum;

	//								if (oneLine[i - 1] == 'i' || oneLine[i] == 'i')
	//									newNum.im = stoi(tempNum);
	//								else if (oneLine[i - 1] == '/')
	//									newNum.dem = stoi(tempNum);
	//								else
	//									newNum.re = stoi(tempNum);

	//								// add to complex number stack
	//								complex.addBack(newNum);

	//								// reset 
	//								tempNum = "";
	//							}

	//							// add to operators stack
	//							if (oneLine[i] != 'i')
	//							operators.addBack(oneLine[i]);

	//							//determine whether to perform arithmetic operation or continue
	//							//___________________________________________________________

	//							// if there  r parentheses and an operation that needs to take place
	//							if (operators.findNodeBack(')') != nullptr) 
	//							{
	//								// holds the locations of the last complete operations in parentheses
	//								/*DoublyLinkedListIterator<char> startOfOp(operators.findNodeBack( '(' ));
	//								DoublyLinkedListIterator<char> endOfOp(operators.findNodeBack( ')' ));*/
	//								nodeType<char>* startOfOp(operators.findNodeBack( '(' )); 
	//								nodeType<char>* endOfOp(operators.findNodeBack(')'));

	//								// holds the current operation as a string
	//								string equation = "";

	//								// stop when you pop all the way to the closed parenthese
	//								while (startOfOp != endOfOp)
	//								{
	//								
	//										// don't add the parentheses to equation 
	//										if (startOfOp->data == '('
	//											|| startOfOp->data == ')')
	//										{
	//											// update loop condition
	//											startOfOp = startOfOp->next;

	//											// delete operator
	//											operators.popFront();
	//											continue;
	//										}

	//										// perform calculation
	//										else
	//										{
	//											switch (startOfOp->data)
	//											{
	//												case '+':
	//												{
	//													Complex sum = complex.begin().current->data + complex.begin().current->next->data;
	//										
	//													/*
	//													delete complex numbers (2x because after the first one will be deleted the next will
	//													become the beginning (first element in the linked list (stack)
	//													*/
	//													complex.popFront();
	//													complex.popFront();

	//													// add node to the complex container
	//													complex.addBack(sum);
	//												}
	//												break;

	//												case '-':
	//												{
	//													Complex difference = complex.begin().current->data - complex.begin().current->next->data;
	//										
	//													/*
	//													delete complex numbers (2x because after the first one will be deleted the next will
	//													become the beginning (first element in the linked list (stack)
	//													*/
	//													complex.popFront();
	//													complex.popFront();

	//													// add node to the complex container
	//													complex.addBack(difference);
	//												}
	//												break;

	//												case '*':
	//												{
	//													Complex product = complex.begin().current->data * complex.begin().current->next->data;

	//													/*
	//													delete complex numbers (2x because after the first one will be deleted the next will
	//													become the beginning (first element in the linked list (stack)
	//													*/
	//													complex.popFront();
	//													complex.popFront();

	//													// add node to the complex container
	//													complex.addBack(product);
	//												}
	//												break;

	//												case '/':
	//												{
	//													Complex quotient = complex.begin().current->data / complex.begin().current->next->data;

	//													/*
	//													delete complex numbers (2x because after the first one will be deleted the next will
	//													become the beginning (first element in the linked list (stack)
	//													*/
	//													complex.popFront();
	//													complex.popFront();

	//													// add node to the complex container
	//													complex.addBack(quotient);
	//												}
	//												break;

	//												case '%':
	//												{
	//													Complex conjugate = complex.end().current->data;
	//													conjugate.im *= -1;

	//													/*
	//													delete complex number only one operand so one time
	//													*/
	//													complex.popFront();

	//													// add node to the complex container
	//													complex.addBack(conjugate);

	//												}
	//												break;

	//												default:
	//												{
	//													cout << "Operand doesn't exist";
	//													system("exit"); // close program
	//												}

	//											} // end switch

	//										} // end else

	//										//update loop condition
	//										startOfOp = startOfOp->next;

	//										// delete operator
	//										operators.deleteNode(startOfOp->prev->data, startOfOp->prev);

	//									

	//								} // end while

	//								// delete last parenthesis ')'
	//								operators.deleteNode(startOfOp->data, startOfOp);
	//							
	//							}

	//							// no parentheses but operation needs to take place (1-1 relationship complex to operator)
	//							else if (operators.end().current->data == '%')
	//							{
	//							Complex conjugate = complex.end().current->data;
	//							conjugate.im *= -1;

	//							/*
	//							delete complex number only one operand so one time
	//							*/
	//							complex.popBack();

	//							// add node to the complex container
	//							complex.addBack(conjugate);
	//							}

	//							// no parentheses but operation needs to take place (2- at least 1 relationship complex to operator)
	//							else if (complex.getSize() > 1)
	//								  {
	//										switch (operators.begin().current->data)
	//										{
	//											case '+':
	//											{
	//												Complex sum = complex.begin().current->data + complex.begin().current->next->data;

	//												/*
	//												delete complex numbers (2x because after the first one will be deleted the next will
	//												become the beginning (first element in the linked list (stack)
	//												*/
	//												complex.popFront();
	//												complex.popFront();

	//												// add node to the complex container
	//												complex.addBack(sum);
	//											}
	//											break;

	//											case '-':
	//											{
	//												Complex difference = complex.begin().current->data - complex.begin().current->next->data;

	//												/*
	//												delete complex numbers (2x because after the first one will be deleted the next will
	//												become the beginning (first element in the linked list (stack)
	//												*/
	//												complex.popFront();
	//												complex.popFront();

	//												// add node to the complex container
	//												complex.addBack(difference);
	//											}
	//											break;

	//											case '*':
	//											{
	//												Complex product = complex.begin().current->data * complex.begin().current->next->data;

	//												/*
	//												delete complex numbers (2x because after the first one will be deleted the next will
	//												become the beginning (first element in the linked list (stack)
	//												*/
	//												complex.popFront();
	//												complex.popFront();

	//												// add node to the complex container
	//												complex.addBack(product);
	//											}
	//											break;

	//											case '/':
	//											{
	//												Complex quotient = complex.begin().current->data / complex.begin().current->next->data;

	//												/*
	//												delete complex numbers (2x because after the first one will be deleted the next will
	//												become the beginning (first element in the linked list (stack)
	//												*/
	//												complex.popFront();
	//												complex.popFront();

	//												// add node to the complex container
	//												complex.addBack(quotient);
	//											}
	//											break;

	//											default:
	//											{
	//												cout << "Operand doesn't exist";
	//												system("exit"); // close program
	//											}

	//										} // end switch
	//								  } // end of last else if

	//					  } // end of if the character is an operator


	//			 // stop when encountering an equals sign
	//				else if (oneLine[i] == '=')
	//				break;

	//			} // end for loop

	//		} // end while not end of file
	//	} // end if the file is open

	//} // end if the file exists


// ABOVE IS HOW IT USE TO BE WHERE IT WORKS UNTIL YOU INTRODUCE SUMN LIKE (33+ 2i)*4+i cuz doesn't consider 4+i one complex number so only multiplies by 4

	if (inFile.good() && outFile.good())
	{
		if (inFile.is_open() && outFile.is_open()) // if file is open
		{
			while (!inFile.eof()) // while not eof
			{
				// process the line and make it into a complex number
				//perform necessary operations then output to result.txt
				getline(inFile, oneLine);
				oneLine = noWhiteSpace(oneLine);
				oneLine = insertParen(oneLine);

				int swap = -2; // iteration of last swap
				int j = 0; // number of iterations for calculation 
				char prevOperator = '\0'; // keeps track of previous operator

				//  loop through oneLine until it has been fully processed
				for (int i = 0; i < oneLine.length(); i++)
				{
					// if character is a space 
					if (oneLine[i] == 32)
						continue;

					//if character is an i with digit in front
					if (oneLine[i] == 'i')
						if (isdigit(oneLine[i - 1]) == true)
							continue;

					// if character is an i with no digit in front
					if (oneLine[i] == 'i'
						&& isdigit(oneLine[i - 1]) == false)
					{
						tempNum += '1';
					}

					// if the character is a digit
					if (isdigit(oneLine[i]))
					{
						// add number to tempNum to make complex number
						tempNum += oneLine[i];
						continue; // back up to update statement
					}

					// if character is an operator
					else if (isOperator(oneLine[i]) == true
						|| isLeftParen(oneLine[i]) == true
						|| isRightParen(oneLine[i]) == true
						|| (tempNum == "1" && oneLine[i] == 'i')
						|| (tempNum == "-1" && oneLine[i] == 'i'))
					{
						// time to create complex number 
						if (i > 0 && tempNum != ""
							&& oneLine[i] != '%') // so won't access array position less than 0
						{
							// add to complex number stack
							complex.addBack(createComplexNum(oneLine, i, tempNum));

							// reset 
							tempNum = "";
						}

						// add to operators stack
						if (oneLine[i] != 'i'
							|| isLeftParen(oneLine[i]) == true
							|| isRightParen(oneLine[i]) == true) // previously were false set to true to add them to operators stack
							operators.addBack(oneLine[i]);

					} // end of if the character is an operator


		   // need to perform all calculations
					else if (oneLine[i] == '=')
					{
						if (operators.getSize() == 0) // no operators  
						{

							cout << Complex(0, 0, 1);
						}

						if (tempNum != "")
						{
							// add to complex number stack
							complex.addBack(createComplexNum(oneLine, i, tempNum));

							// reset 
							tempNum = "";
						}

						// convert to postfix notation
						operators = operatorToPostfixParen(operators);
						Complex answer = performOperation(complex, operators).begin().current->data;
						cout << endl << oneLine << endl;
						cout << answer.toString() << endl;
						outFile << answer.toString() << endl;

						// clear complex and operators
						complex.intializeList();
						operators.intializeList();

					} // end if we are at  an equal sign

				} // end for loop to process oneLine in the file
			} // end if the file is not eof

			// close files
			inFile.close();
			outFile.close();

		} // end if the file is open

	} // end of if file exists


	printf("\n\n");
	system("pause");
} // end main

