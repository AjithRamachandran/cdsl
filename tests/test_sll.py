import unittest

from cdsl import sll, sllnode

class TestSll(unittest.TestCase):
    def test_sll(self):
        new_sll = sll()
        new_sll.insert(1)
        new_sll.insert(2)
        new_sll.insert(3)
        new_sll.insert(4)
        new_sll.insert(5)
        new_sll.insert(6)
        new_sll.insert(7)

        new_sll_ = sll()
        new_sll_.insert(8)
        new_sll_.insert(9)
        new_sll_.insert(10)
        new_sll_.insert(11)
        new_sll_.insert(12)
        new_sll_.insert(13)
        new_sll_.insert(14)

        new_sll.extend(new_sll_)
        new_sll.extend(new_sll)

        print(new_sll)

        self.assertEqual(0, 0)

class TestSllNode(unittest.TestCase):
    def test_sll(self):
        new_sll = sll()
        new_sll.insert(1, 0)
        new_sll.insert(2, 0)
        new_sll.insert(3, 0)
        new_sll.insert(4, 0)
        new_sll.insert(5, 0)
        new_sll.insert(6, 0)
        print(new_sll, new_sll.length, new_sll.head, new_sll.tail)
        new_sllnode = sllnode(10)
        print(new_sllnode, new_sllnode.value, new_sllnode.next )
        new_sll.insert_node(new_sllnode, 0)
        print(new_sllnode, new_sllnode.value, new_sllnode.next )
        print(new_sll, new_sll.length, new_sll.head, new_sll.tail)
        self.assertEqual(0, 0)



if __name__ == '__main__':
    unittest.main()
