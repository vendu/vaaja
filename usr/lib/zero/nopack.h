/* REFERENCE: https://stackoverflow.com/questions/1537964/visual-c-equivalent-of-gccs-attribute-packed */

#undef PACKED
#if defined(_MSC_VER)
__pragma(pack(pop))
#endif

