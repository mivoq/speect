.. _errdbg_topic/main:

============================
Error handling and Debugging
============================

The error handling mechanism works on the basis that *all* [#]_ functions are passed
a pointer to an error code variable, :c:type:`s_erc`,  as a function parameter. If an
error occurred during function execution, then the function can set an appropriate error
code in this pointer. This of course requires us to *check everything everywhere*. 
Basically, this consists in assuming that nearly every function call might fail for some
reason. This scheme is simple, easy to understand, straightforward to implement and it
is transparent, there are no ``setjmp``/``longjmp`` type macros that might hide
implementation detail and make debugging a nightmare. It doesn't require runtime support,
is highly portable, and supports multiple-threads by requiring only the most basic
synchronisation primitives in most code. However, it demands discipline and commitment
to check all function calls when writing, and maintaining code.


.. code-block:: c

   int main(float *d)
   {
	s_erc error = S_SUCCESS; /* start of with a clean slate */
	size_t word_length;


	/* initialize speect */
	error = speect_init(NULL);
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}
   }

   s_erc error; /* declare the error return code variable */


   S_CLR_ERR(&error); /* clear the error return code */

The macro :c:type:`S_CLR_ERR` will check that ``error`` is not ``NULL`` and set it to ``S_SUCCESS``.
It is good practice to always clear the error return code at the start of a function.





   

All calling functions can then check the error pointer and take appropriate action based
on the error code. Each calling function can then add context to the error and in turn
set an error code for it's calling function again, thereby giving a detailed trace from
the origin of the error to the source of the error.

Let us look at an example of using error handling and debugging.



.. literalinclude:: ../../../engine/examples/base/errdbg/errdbg_example_short.c
   :language: c
   :linenos:
   :lines: 36-	


There are two types of errors, *fatal* and *non-fatal* errors. Fatal errors
are usually programming errors, memory errors and so forth, where there are no alternative
options if such an error occured. Non-fatal errors are errors that occured from which
the system can recover or reset to a previous state and continue.

The error handling system as described enables one to have a detailed "backtrace" that has
been logged to some or other place, and therefore makes development and bug tracking a much easier
task.

.. [#] There are a few exceptions to this rule, especially where errors might occur before
       the *Error Handling And Debugging* module has been initialized.


.. code-block:: python

    class TTSClient(object):
        END_OF_MESSAGE_STRING = "<EoM>"

        def __init__(self, host=DEF_HOST, port=DEF_PORT, recv_size=RECV_SIZE):
            self.host = host
            self.port = port
            self.recv_size = recv_size

        def request(self, rtype="listvoices", voice=None, text=None):
            try:
                text = unicode(text, "latin-1").encode("utf-8")
            except TypeError:
                pass
            message = {"type": rtype,
                       "voicename": voice,
                       "text": text}

            fulls = pickle.dumps(message)
            #create a socket
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            #connect to server
            s.connect((self.host, self.port))
            #send..
            s.sendall(fulls)
            s.sendall(TTSClient.END_OF_MESSAGE_STRING)
            #recv reply..
            replymsgfull = str()
            while True:
                replymsgpart = s.recv(self.recv_size)
                if replymsgpart:
                    replymsgfull += replymsgpart
                else:
                    break
            #close connection..
            s.close()
            #recover reply..
            reply = pickle.loads(replymsgfull)
            if not reply["success"]:
                raise Exception("Request failed..")

            return reply

        def write_audio(self, voice, text, filename):
            reply = self.request("synth", voice, text)
            if reply["sampletype"] != "int16":
                raise Exception("Client currently only supports 16bit samplesize")
            outwf = wave.open(filename, "w")
            outwf.setparams((1, 2, reply["samplerate"], len(reply["samples"]) / 2, "NONE", "not compressed"))
            outwf.writeframesraw(reply["samples"])
        outwf.close()
