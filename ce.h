/** Simple nested exceptions for C
	author: Rafał Przywara <ptah.peteh@gmail.com>, (c) 2007
	license: MIT

	usage:

	TRY {
		...
		if (some + thing - goes == wrong) THROW(-1);
		...
		this_can_throw_too();
		...
	}
	CATCH (ex) {
		// ex: exception code passed to THROW(), int
	}
	FINALLY {
		// this gets always executed after try/catch blocks
	}

	CATCH and FINALLY are obligatory after TRY. TRY blocks can be nested.

	Throw value is an int error code other than 0. Passing 0 to THROW() or
	throwing exception outside of any TRY block will abort().

	Directive USE_EXCEPTIONS; has to be placed somewhere in the global scope.
	Compile with -DCE_M for multithread support.
*/
#include <setjmp.h>
#include <stdlib.h>

#ifndef __ce_h__
#define __ce_h__

/// multithread support
#ifdef CE_M
#define CE_TLS _Thread_local
#else
#define CE_TLS
#endif

/** holds info about an exception handler (linked list)
*/
typedef struct ce_exception__ {
	jmp_buf env;						///< exception handler environment
	struct ce_exception__ *prev;		///< pointer to the previously set handler
} ce_exception_;

/// pointer to the current (most recent) exception handler
extern CE_TLS ce_exception_ *ce_glob_;

/// should be placed spmewhere in the program, only once
#define USE_EXCEPTIONS CE_TLS ce_exception_ *ce_glob_ = NULL

/// throws an exception of code e, e must no be 0
#define THROW(e) if (ce_glob_ && e) longjmp(ce_glob_->env, e); else abort()

/// begins try block
#define TRY void ce_try_(void)

/// begins catch block
#define CATCH(e) void ce_catch_(int e)

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
