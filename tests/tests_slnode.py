import unittest

from cdsl import slnode

class TestSllNode(unittest.TestCase):
    def test_sll(self):
        new_sllnode = slnode(10)
        print(new_sllnode, new_sllnode.value, new_sllnode.next )
        self.assertEqual(0, 0)


if __name__ == '__main__':
    unittest.main()
