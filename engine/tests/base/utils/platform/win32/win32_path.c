/******************************************************************************/
/* Copyright (c) 2017 Mivoq SRL <info@mivoq.it>                               */
/*                                                                            */
/* Contributors: Giulio Paci                                                  */
/*                                                                            */
/* Permission is hereby granted, free of charge, to any person obtaining a    */
/* copy of this software and associated documentation files (the "Software"), */
/* to deal in the Software without restriction, including without limitation  */
/* the rights to use, copy, modify, merge, publish, distribute, sublicense,   */
/* and/or sell copies of the Software, and to permit persons to whom the      */
/* Software is furnished to do so, subject to the following conditions:       */
/* The above copyright notice and this permission notice shall be included in */
/* all copies or substantial portions of the Software.                        */
/*                                                                            */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    */
/* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    */
/* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        */
/* DEALINGS IN THE SOFTWARE.                                                  */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* AUTHOR  : Giulio Paci <giulio.paci@mivoq.it>                               */
/* DATE    : December 2017       	                                      */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* Path unit-test                                                             */
/*                                                                            */
/******************************************************************************/

#include <speect.h>
#include <base/utils/platform/win32/win32_path.h>
#include <stdio.h>

static int test_expected_results(FILE* results, FILE* log, s_erc error, const char* input, const char* result, const char* expected, int* success_counter) {
	if (error != S_SUCCESS) {
		fprintf(stderr, "failed (call to function failed)\n");
		return 1;
	}
	if(result != NULL) {
		if(expected != NULL) {
			if(!s_strcmp(result, expected, &error)) {
				fprintf(results, "ok (got %s, expected %s, from %s)\n", result, expected, input);
			} else {
				fprintf(results, "failed (got %s, expected %s, from %s)\n", result, expected, input);
				return -1;
			}
		} else {
			fprintf(results, "failed (got %s, expected NULL, from %s)\n", result, input);
			return -1;
		}
	} else {
		if(expected != NULL) {
			fprintf(results, "failed (got NULL, expected %s, from %s)\n", expected, input);
			return -1;
		}
	}
	if(success_counter != NULL) {
		*success_counter = *success_counter + 1;
	}
	return 0;
	(void) log;
}

static int test_speect_s_win32_get_base_path(FILE* results, FILE* log, const char* path, const char* expected_base_path, int* success_counter) {
	s_erc error = S_SUCCESS;
	int ret = 0;
	char* basepath = s_win32_get_base_path(path, &error);
	ret = test_expected_results(results, log, error, path, basepath, expected_base_path, success_counter);
	S_FREE(basepath);
	return ret;
}

static int test_speect_s_win32_path_combine(FILE* results, FILE* log, const char* path, const char* filename, const char* expected_base_path, int* success_counter) {
	s_erc error = S_SUCCESS;
	int ret = 0;
	char* basepath = s_win32_path_combine(path, filename, &error);
	ret = test_expected_results(results, log, error, path, basepath, expected_base_path, success_counter);
	S_FREE(basepath);
	return ret;
}


int main(int argc, char** argv) {
	s_erc error = S_SUCCESS;
	int successes = 0;
	int tests = 0;
	(void) argc;
	(void) argv;
	/*
	 * initialize speect
	 */
	error = speect_init(NULL);
	if (error != S_SUCCESS) {
		fprintf(stderr, "Failed to initialize Speect\n");
		return 1;
	}
	successes = 0;
	tests = 0;
	test_speect_s_win32_get_base_path(stdout, stderr, "/", "/", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "/tmp", "/", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "/tmp/", "/", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "/tmp//", "/", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "/tmp/tmp", "/tmp", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "/tmp/tmp/", "/tmp", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "/tmp/tmp//", "/tmp", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "/tmp//tmp//", "/tmp", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, NULL, ".", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "", ".", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, ".", ".", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "..", ".", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "./", ".", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "../", ".", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "tmp", ".", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "tmp/", ".", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "tmp//", ".", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "tmp/tmp", "tmp", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "tmp/tmp/", "tmp", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "tmp/tmp//", "tmp", &successes); tests++;
	test_speect_s_win32_get_base_path(stdout, stderr, "tmp//tmp//", "tmp", &successes); tests++;
	fprintf(stdout, "%d/%d\n", successes, tests);
	successes = 0;
	tests = 0;
	test_speect_s_win32_path_combine(stdout, stderr, "/", "file", "/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "/tmp", "file", "/tmp/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "/tmp/", "file", "/tmp/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "/tmp//", "file", "/tmp/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "/tmp", "", "/tmp/", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "/tmp/", "", "/tmp/", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "/tmp", NULL, "/tmp/", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "/tmp/", NULL, "/tmp/", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "tmp", "file", "tmp/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "tmp/", "file", "tmp/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "tmp//", "file", "tmp/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "tmp", "", "tmp/", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "tmp/", "", "tmp/", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "tmp", NULL, "tmp/", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "tmp/", NULL, "tmp/", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, NULL, "file", "file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "", "file", "file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, ".", "file", "./file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "..", "file", "../file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "./", "file", "./file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "../", "file", "../file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "/tmp/", "/abs/file", "/abs/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "/tmp/", "file:///abs/file", "file:///abs/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "/tmp/", "file:tmp/file", "/tmp/file:tmp/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "http://www.aaa.aa/", "file", "http://www.aaa.aa/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "http://www.aaa.aa", "file", "http://www.aaa.aa/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "http://www.aaa.aa:80/", "file", "http://www.aaa.aa:80/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "http://www.aaa.aa:80", "file", "http://www.aaa.aa:80/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "http://www.aaa.aa:80/abc/def/", "file", "http://www.aaa.aa:80/abc/def/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "http://www.aaa.aa:80/abc/def", "file", "http://www.aaa.aa:80/abc/def/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "ààà", "file", "ààà/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "ààà/", "file", "ààà/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "ààà//", "file", "ààà/file", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "ààà", "", "ààà/", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "ààà/", "", "ààà/", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "ààà", NULL, "ààà/", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "ààà/", NULL, "ààà/", &successes); tests++;

	test_speect_s_win32_path_combine(stdout, stderr, "/tmp", "/file:test", "/file:test", &successes); tests++;
	test_speect_s_win32_path_combine(stdout, stderr, "/tmp", "f+0:/test", "f+0:/test", &successes); tests++;
	fprintf(stdout, "%d/%d\n", successes, tests);
	/*
	 * quit speect
	 */
	error = speect_quit();
	if (error != S_SUCCESS)
	{
		fprintf(stderr, "Call to 'speect_quit' failed\n");
		return 1;
	}
	return 0;
}
