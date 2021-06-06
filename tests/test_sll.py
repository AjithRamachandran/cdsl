import unittest

from cdsl import sll, slnode

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
        print(new_sll.node_at(5))
        print(new_sll.search(5), new_sll.search(15))
        
        new_sllnode = slnode(10)
        new_sll.insert_node(new_sllnode)
        print(new_sll, new_sll.tail, new_sll.tail.next)

        self.assertEqual(0, 0)


if __name__ == '__main__':
    unittest.main()
