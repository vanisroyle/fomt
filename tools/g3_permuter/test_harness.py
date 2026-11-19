import unittest

from prepare_target import extract_function
from select_section import select_section


class SectionSelectionTests(unittest.TestCase):
    def test_preserves_function_and_pool_only(self):
        section = '.section .text.g3_test,"ax",%progbits\nfn:\n bx lr\n.word 123\n'
        self.assertEqual(
            select_section('.text\nother:\n bx lr\n' + section + '.section .debug_info\n', '.text.g3_test'),
            '\t.code 16\n' + section,
        )

    def test_empty_section(self):
        self.assertEqual(
            select_section('.section .text.g3_test\n', '.text.g3_test'),
            '\t.code 16\n.section .text.g3_test\n',
        )

    def test_missing_section(self):
        with self.assertRaises(ValueError):
            select_section('', '.text.g3_test')

    def test_requires_exact_section_name(self):
        with self.assertRaises(ValueError):
            select_section('.section .text.g3_test_extra\n', '.text.g3_test')


class TargetExtractionTests(unittest.TestCase):
    def test_stops_before_next_entry(self):
        body = '\tthumb_func_start first\nfirst:\n push {lr}\n pop {pc}\n'
        self.assertEqual(
            extract_function(body + '\tthumb_func_start second\nsecond:\n bx lr\n', 'first'),
            body,
        )

    def test_stops_before_next_section(self):
        body = '\tthumb_func_start first\nfirst:\n bx lr\n'
        self.assertEqual(extract_function(body + '\t.section .text.next\n', 'first'), body)

    def test_rejects_internal_label(self):
        with self.assertRaises(ValueError):
            extract_function('\tthumb_func_start parent\nparent:\ninternal:\n bx lr\n', 'internal')

    def test_empty_source(self):
        with self.assertRaises(ValueError):
            extract_function('', 'missing')


if __name__ == '__main__':
    unittest.main()
