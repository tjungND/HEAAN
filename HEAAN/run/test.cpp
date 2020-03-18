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
	long logq = 117; ///< Ciphertext Modulus, only needed to change this for Taeho's experiment
    
    /*
     
     
     
     */
    
    
    
    
    
    
	long logp = 16; ///< Real message will be quantized by multiplying 2^(log p + 10)
	long logn = 10; ///< log2(The number of slots)
	long n = (1 << logn);
	srand(time(NULL));
    cout << endl;
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

	timeutils.start("Homomorphic Multiplication for 10 times");
//	timeutils.start("Homomorphic Multiplication");
	for(long i = 0; i < 10; i++){
		scheme.multAndEqual(cipher[rand() % 100], cipher[rand() % 100]);
	}
	timeutils.stop("Homomorphic Multiplication for 10 times");
//	timeutils.stop("Homomorphic Multiplication");
    
    cout << "Avg Mult. time is : " << (timeutils.timeElapsed / 10.0) << "ms" << endl;



//----------------------------------------------------------------------------------
//   BOOTSTRAPPING
//----------------------------------------------------------------------------------

    
    
    int depth = 0;
    int totalq= 0;
    
    logq = logp + 10; //< suppose the input ciphertext of bootstrapping has logq = logp + 10
//    	logn = 1; //< larger logn will make bootstrapping tech much slower
    long logT = 4; //< this means that we use Taylor approximation in [-1/T,1/T] with double angle fomula
    
    for(int i = 0; i < 100; i++){
        scheme.encrypt(cipher[i], mvec, n, logp, logq);
    }
    
	
//	cout << "cipher's logq before bootstrapping = " << cipher.logq << endl;
//	timeutils.start("Bootstrapping");
	timeutils.start("Bootstrapping for 10 times");
	
	for(long i = 0; i < 1; i++){
        int j = rand() % 100;
        //cout << "iteration " << i << endl;
		scheme.bootstrapAndEqual(cipher[j], logq, logQ, logT);
        
		depth += (cipher[j].logq / logp - 1);
		totalq += cipher[j].logq;
	}
	timeutils.stop("Bootstrapping for 10 times");
//	timeutils.stop("Bootstrapping");
    cout << "Avg Boot. time is : " << (timeutils.timeElapsed / 1.0) << "ms" << endl;
	cout << "avg depth = " << (depth / 1.0) << endl;
	cout << "avg logq = " << (totalq / 1.0) << endl;
    cout << endl;


//    	TestScheme::testBootstrap(logq, logp, logn, logT);

	return 0;
}
