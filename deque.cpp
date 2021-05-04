/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

#include <stack>

template <class T>
Deque<T>::Deque()
{
    /* YOUR CODE HERE! */
    n1 = 0;
    n2 = 0;
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    /**
     * @todo Your code here!
     */
    data.push_back(newItem);
    n2++;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    /**
     * @todo Your code here! 
     */

    T left_element = peekL();
    n1++;
    int num_elements = n2 - n1; //if n1 points to 6, n1 = 5 and n2 = 10

    if (n1 >= num_elements)
    {
        // double check the num of elements and number of times to run
        vector<T> temp_v;

        for (int i = n1; i < n2; i++)
        {
            temp_v.push_back(data[i]);
        }

        data = temp_v;
        n1 = 0;
        n2 = data.size();
    }
    return left_element;
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    /**
     * @todo Your code here! You will need to replace the following line.
     */
    T right_element = peekR();
    data.pop_back();
    n2--;

    int num_elements = n2 - n1;
    if (n1 >= num_elements)
    {
        // double check the num of elements and number of times to run
        vector<T> temp_v;

        for (int i = n1; i < n2; i++)
        {
            temp_v.push_back(data[i]);
        }

        data = temp_v;
        n1 = 0;
        n2 = data.size();
    }

    return right_element;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    /**
     * @todo Your code here! 
     */

    T my_front = data[n1];

    return my_front;
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    /**
     * @todo Your code here! 
     */

    T back = data[n2 - 1];
    return back;
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    /**
     * @todo Your code here! 
     */
    return n2 <= n1;
}
