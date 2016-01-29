/** Simple nested exceptions for C
	author: Rafał Przywara <ptah.peteh@gmail.com>, (c) 2007
	license: MIT

	usage:

	TRY {
		...
		if (some + thing - goes == wrong) THROW(-1);
		...
	}
	CATCH (ex) {
		// ex: exception code passed to THROW()
	}
	FINALLY {
		// this gets always executed after try/catch blocks
	}
*/
#include <setjmp.h>
#include <stdlib.h>

#ifndef __ce_h__
#define __ce_h__

/** holds info about an exception handler (linked list)
*/
typedef struct ce_exception__ {
	jmp_buf env;						///< exception handler environment
	struct ce_exception__ *prev;		///< pointer to the previously set handler
} ce_exception_;

/// pointer to the current (most recent) exception handler
extern ce_exception_ *ce_glob_;

/// should be placed spmewhere in the program, only once
#define USE_EXCEPTIONS ce_exception_ *ce_glob_ = NULL

/// throws an exception of code e, e must no be 0
#define THROW(e) if (ce_glob_ && e) longjmp(ce_glob_->env, e); else abort()

/// begins try block
#define TRY auto void ce_try_(void)

/// begins catch block
#define CATCH(e) auto void ce_catch_(int e)

/// begins finally block
#define FINALLY auto void ce_finally_(void); {	\
	ce_exception_ lce_;							\
	int ec_;									\
												\
	lce_.prev = ce_glob_;						\
	ce_glob_ = &lce_;							\
												\
	if ((ec_ = setjmp(ce_glob_->env))) {		\
		int cc_;								\
												\
		if ((cc_ = setjmp(ce_glob_->env))) {	\
			ce_glob_ = lce_.prev;				\
			ce_finally_();						\
			if (!ce_glob_) abort();				\
			longjmp(ce_glob_->env, cc_);		\
		} else {								\
			ce_catch_(ec_);						\
		}										\
	} else {									\
		ce_try_();								\
	}											\
												\
	ce_glob_ = lce_.prev;						\
	ce_finally_();								\
} 												\
void ce_finally_()								\

#endif // __ce_h__
