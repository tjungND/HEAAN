/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/

#include "../src/HEAAN.h"
/**
  * This file is for test HEAAN library
  * You can find more in src/TestScheme.h
  * "./TestHEAAN Encrypt" will run Encrypt Test
  * There are Encrypt, EncryptSingle, Add, Mult, iMult, RotateFast, Conjugate Tests
  */
int main(int argc, char **argv) {
	long logq = 420; ///< Ciphertext Modulus
	long logp = 16; ///< Real message will be quantized by multiplying 2^(log p + 10)
	long logn = 0; ///< log2(The number of slots)
	long n = (1 << logn);
	srand(time(NULL));

	cout << "message precision = " << logp << endl;
    	cout << "log(poly-degree, i.e., n) = " << logN << endl;
    	cout << "log(temporary expansion) = " << logQ << endl;


    	cout << "log(cipher modulus, i.e., q) = " << logq << endl;
	cout << "multiplicative depth = " << (logq / logp - 1) << endl;
	cout << "# of packed messages = " << n << endl;

	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring;
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);


//	timeutils.start("BootKeyGen (one-time)");
	scheme.addBootKey(secretKey, logn, logp + 10 + 4);
//	timeutils.stop("BootKeyGen (one-time)");


//----------------------------------------------------------------------------------
//   MULT TESTS
//----------------------------------------------------------------------------------

//	TestScheme::testMult(logq, logp, logn);





	complex<double>* mvec = EvaluatorUtils::randomComplexArray(n);


	Ciphertext* cipher = new Ciphertext[100];
	for(int i = 0; i < 100; i++){
		scheme.encrypt(cipher[i], mvec, n, logp, logq);
	}

	timeutils.start("Homomorphic Multiplication for 100 times");
//	timeutils.start("Homomorphic Multiplication");
	for(long i = 0; i < 100; i++){
		scheme.multAndEqual(cipher[rand()], cipher[rand()]);
	}
	timeutils.stop("Homomorphic Multiplication for 100 times");	 
//	timeutils.stop("Homomorphic Multiplication");	

    
//----------------------------------------------------------------------------------
//   BOOTSTRAPPING
//----------------------------------------------------------------------------------


    	logq = (logq % logp) + logp; //< suppose the input ciphertext of bootstrapping has logq = logp + 10
//    	logn = 1; //< larger logn will make bootstrapping tech much slower
    	long logT = 4; //< this means that we use Taylor approximation in [-1/T,1/T] with double angle fomula

	
//	cout << "cipher's logq before bootstrapping = " << cipher.logq << endl;
//	timeutils.start("Bootstrapping");
	timeutils.start("Bootstrapping for 100 times");
	int depth = (logq / logp - 1);
	int totalq= logq;
	for(long i = 0; i < 1; i++){
		scheme.bootstrapAndEqual(cipher[rand()], cipher[rand()].logq, logQ, logT);
		depth += (cipher[rand()].logq / logp - 1);
		totalq += cipher[rand()].logq;
	}
	timeutils.stop("Bootstrapping for 100 times");
//	timeutils.stop("Bootstrapping");
	cout << "avg depth = " << (depth / 100.0) << endl;
	cout << "avg logq = " << (totalq / 100.0) << endl;


//    	TestScheme::testBootstrap(logq, logp, logn, logT);

	return 0;
}
