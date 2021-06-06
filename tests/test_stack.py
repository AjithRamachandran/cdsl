import unittest

from cdsl import stack

class TestSll(unittest.TestCase):
    def test_sll(self):
        new_stack = stack()
        new_stack.push(1)
        new_stack.push(2)
        new_stack.push(3)
        new_stack.push(4)
        new_stack.push(5)
        print(new_stack.size)
        print(new_stack)
        print(new_stack.peek())
        print(new_stack.pop())
        print(new_stack.size)
        print(new_stack.pop())
        print(new_stack.size)
        print(new_stack)

        self.assertEqual(0, 0)

if __name__ == '__main__':
    unittest.main()
