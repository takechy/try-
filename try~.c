#include "ext.h"
#include "z_dsp.h"

typedef struct _try {
	t_pxobject m_obj;
} t_try;


void *try_new(t_symbol *s,  long argc, t_atom *argv);
//void try_free(t_try *x);
//t_max_err try_notify(t_try *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
//void try_assist(t_try *x, void *b, long m, long a, char *s);
//void try_limits(t_try *x);
//void try_set(t_try *x, t_symbol *s, long ac, t_atom *av);
//void try_float(t_try *x, double f);
//void try_int(t_try *x, long n);
//void try_dblclick(t_try *x);
void try_perform64(t_try *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void try_dsp64(t_try *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);


static t_symbol *ps_buffer_modified;
static t_class *s_try_class;


void ext_main(void *r)
{
    t_class *c = class_new("try~", (method)try_new, (method)dsp_free, sizeof(t_try), NULL, 0);

	class_addmethod(c, (method)try_dsp64,		"dsp64",	A_CANT, 0);
//    class_addmethod(c, (method)try_float,        "float",    A_FLOAT, 0);
//    class_addmethod(c, (method)try_int,        "int",        A_LONG, 0);
//    class_addmethod(c, (method)try_set,        "set",        A_GIMME, 0);
//    class_addmethod(c, (method)try_assist,        "assist",    A_CANT, 0);
//    class_addmethod(c, (method)try_dblclick,    "dblclick",    A_CANT, 0);
//    class_addmethod(c, (method)try_notify,        "notify",    A_CANT, 0);

	class_dspinit(c);
	class_register(CLASS_BOX, c);
	s_try_class = c;

	ps_buffer_modified = gensym("buffer_modified");
}


void *try_new(t_symbol *s,  long argc, t_atom *argv)
{
	t_try *x = (t_try *)object_alloc(s_try_class);
//    t_symbol *buf=0;
//    float try=0., end=0.;
//    double msr = sys_getsr() * 0.001;

	dsp_setup((t_pxobject *)x,1);
//    buf = atom_getsymarg(0,argc,argv);
//    try = atom_getfloatarg(1,argc,argv);
//    end = atom_getfloatarg(2,argc,argv);
//
//    x->w_name = buf;
//    x->w_try = try;
//    x->w_end = end;
//    x->w_begin = try * msr;
//    x->w_len = (end - try) * msr;
	
    //outlet_new((t_object *)x, "signal");		// audio outlet

	// create a new buffer reference, initially referencing a buffer with the provided name
//    x->w_buf = buffer_ref_new((t_object *)x, x->w_name);

	return (x);
}


void try_perform64(t_try *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    t_double        *in = ins[0];
    int       n = sampleframes;
    t_double  value;

    while (n--) {               // perform calculation on all samples
        value = *in++;
        
        post("%f", value);
    }
}


void try_dsp64(t_try *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp64, gensym("dsp_add64"), x, try_perform64, 0, NULL);
}





// A notify method is required for our buffer reference
// This handles notifications when the buffer appears, disappears, or is modified.
//t_max_err try_notify(t_try *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
//{
//    if (msg == ps_buffer_modified)
//        x->w_buffer_modified = true;
//    return buffer_ref_notify(x->w_buf, s, msg, sender, data);
//}
//
//
//void try_assist(t_try *x, void *b, long m, long a, char *s)
//{
//    if (m == ASSIST_INLET) {    // inlets
//        switch (a) {
//        case 0:    snprintf_zero(s, 256, "(signal) Table Position (from 0 to 1)");    break;
//        case 1:    snprintf_zero(s, 256, "(signal/float) trying Table Location in ms");    break;
//        case 2:    snprintf_zero(s, 256, "(signal/float) Ending Table Location in ms");    break;
//        }
//    }
//    else {    // outlet
//        snprintf_zero(s, 256, "(signal) Output %ld", a+1);
//    }
//}
//
//
//void try_limits(t_try *x)
//{
//    t_buffer_obj *b = buffer_ref_getobject(x->w_buf); // get the actual buffer object from our reference
//
//    if (b) {
//        t_atom_long    channelcount = buffer_getchannelcount(b);        // number of floats in a frame
//        t_atom_long    framecount   = buffer_getframecount(b);            // number of floats long the buffer is for a single channel
//        double        msr             = buffer_getmillisamplerate(b);    // sample rate of the buffer in samples per millisecond
//
//        x->w_begin = (long)(x->w_try * msr) * channelcount;//buffer sr-jkc
//        if (!x->w_end)    {// use entire table, eek!
//            x->w_len = framecount;
//        } else {
//            x->w_len = (x->w_end - x->w_try) * msr; //buffer sr-jkc
//        }
//        // now restrict these values
//        if (x->w_begin < 0)
//            x->w_begin = 0;
//        else if (x->w_begin >= framecount * channelcount)
//            x->w_begin = (framecount - 1) * channelcount;
//        if (x->w_begin + (x->w_len * channelcount) >= framecount * channelcount) {
//            x->w_len = framecount - (x->w_begin / channelcount);
//        }
//    }
//}
//
//
//void try_doset(t_try *x, t_symbol *s, long ac, t_atom *av)
//{
//    t_symbol *name;
//    double try, end;
//
//    name = (ac) ? atom_getsym(av) : gensym("");
//    try = (ac>1) ? atom_getfloat(av+1) : 0.;
//    end = (ac>2) ? atom_getfloat(av+2) : 0.;
//
//    if (try < 0)
//        try = 0;
//    if (end < 0)
//        end = 0;
//    x->w_try = try;
//    x->w_end = end;
//
//    buffer_ref_set(x->w_buf, name);    // change the buffer used by our buffer reference
//    try_limits(x);
//}
//
//
//// calls set the buffer ref should happen on the main thread only
//void try_set(t_try *x, t_symbol *s, long ac, t_atom *av)
//{
//    defer(x, (method)try_doset, s, ac, av);
//}
//
//
//void try_float(t_try *x, double f)
//{
//    long in = proxy_getinlet((t_object *)x);
//
//    if (in == 1) {        // set min
//        if (f < 0)
//            f = 0;
//        if (f > x->w_end)
//            x->w_end = f;
//        x->w_try = f;
//        try_limits(x);
//    }
//    else if (in == 2) {    // set max
//        if (f < 0)
//            f = 0;
//        if (f < x->w_try)
//            x->w_try = f;
//        x->w_end = f;
//        try_limits(x);
//    }
//}
//
//
//void try_int(t_try *x, long n)
//{
//    try_float(x,(double)n);
//}
//
//
//void try_dblclick(t_try *x)
//{
//    buffer_view(buffer_ref_getobject(x->w_buf));
//}



