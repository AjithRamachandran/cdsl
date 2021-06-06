import unittest

from cdsl import queue

class TestSll(unittest.TestCase):
    def test_sll(self):
        new_queue = queue()
        new_queue.enqueue(1)
        new_queue.enqueue(2)
        new_queue.enqueue(3)
        new_queue.enqueue(4)
        new_queue.enqueue(5)
        print(new_queue.size)
        print(new_queue)
        print(new_queue.peek())
        print(new_queue.dequeue())
        print(new_queue.size)
        print(new_queue.dequeue())
        print(new_queue.size)
        print(new_queue)

        self.assertEqual(0, 0)

if __name__ == '__main__':
    unittest.main()
