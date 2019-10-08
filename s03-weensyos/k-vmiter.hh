#ifndef WEENSYOS_K_VMITER_HH
#define WEENSYOS_K_VMITER_HH
#include "kernel.hh"

// `vmiter` and `ptiter` are iterator types for x86-64 page tables.


// `vmiter` walks over virtual address mappings.
// `pa()` and `perm()` read current addresses and permissions;
// `map()` installs new mappings.

class vmiter {
  public:
    // initialize a `vmiter` for `pt` pointing at `va`
    inline vmiter(x86_64_pagetable* pt, uintptr_t va = 0);
    inline vmiter(const proc* p, uintptr_t va = 0);

    inline uintptr_t va() const;      // current virtual address
    inline uintptr_t last_va() const; // one past last va in this range
    inline bool low() const;          // is va low?
    inline uint64_t pa() const;       // current physical address
    template <typename T = void*>
    inline T kptr() const;            // pa() as a pointer
    inline uint64_t perm() const;     // current permissions
    inline bool perm(uint64_t p) const; // are all permissions `p` enabled?
    inline bool perm(uintptr_t last_va, uint64_t p);
    template <typename T = void*>
    inline bool low_perm(T* first, T* last, uint64_t p);
    template <typename T = void*>
    inline bool low_perm(T* first, size_t len, uint64_t p);
    inline bool present() const;      // is va present?
    inline bool writable() const;     // is va writable?
    inline bool user() const;         // is va user-accessible (unprivileged)?

    inline vmiter& find(uintptr_t va);   // change virtual address to `va`
    inline vmiter& operator+=(intptr_t delta);  // advance `va` by `delta`
    inline vmiter& operator-=(intptr_t delta);

    // move to next page-aligned va, skipping large empty regions
    // Never skips present pages.
    void next();

    // move to next page-aligned va with different perm/pa (i.e., `last_va()`)
    // Like next(), but also skips present pages.
    void next_range();

    // map current va to `pa` with permissions `perm`
    // Current va must be page-aligned. Calls `kalloc` to allocate
    // page table pages if necessary. Returns 0 on success,
    // negative on failure.
    int map(uintptr_t pa, int perm = PTE_P | PTE_W | PTE_U)
        __attribute__((warn_unused_result));
    // this version takes a pointer
    inline int map(void* pa_ptr, int perm = PTE_P | PTE_W | PTE_U)
        __attribute__((warn_unused_result));

  private:
    x86_64_pagetable* pt_;
    x86_64_pageentry_t* pep_;
    int level_;
    int perm_;
    uintptr_t va_;

    static constexpr int initial_perm = 0xFFF;
    static const x86_64_pageentry_t zero_pe;

    void down();
    void real_find(uintptr_t va);
};


// `ptiter` walks over the page table pages in a page table,
// returning them in depth-first order.
// This is mainly useful when freeing a page table, as in:
// ```
// for (ptiter it(pt); it.active(); it.next()) {
//     freepage(it.ptp());
// }
// freepage(pt);
// ```
// Note that `ptiter` will never visit the level 4 page table page.

class ptiter {
  public:
    // initialize a `ptiter` for `pt` pointing at `va`
    inline ptiter(x86_64_pagetable* pt, uintptr_t va = 0);
    inline ptiter(const proc* p, uintptr_t va = 0);

    inline uintptr_t va() const;            // current virtual address
    inline uintptr_t last_va() const;       // one past last va covered by ptp
    inline bool active() const;             // does va exist?
    inline bool low() const;                // is va low?
    inline int level() const;               // current level (0-2)
    inline x86_64_pagetable* ptp() const;   // current page table page
    inline uintptr_t ptp_pa() const;        // physical address of ptp

    // move to next page table page in depth-first order
    inline void next();

  private:
    x86_64_pagetable* pt_;
    x86_64_pageentry_t* pep_;
    int level_;
    uintptr_t va_;

    void go(uintptr_t va);
    void down(bool skip);
};


inline vmiter::vmiter(x86_64_pagetable* pt, uintptr_t va)
    : pt_(pt), pep_(&pt_->entry[0]), level_(3), perm_(initial_perm), va_(0) {
    real_find(va);
}
inline vmiter::vmiter(const proc* p, uintptr_t va)
    : vmiter(p->pagetable, va) {
}
inline uintptr_t vmiter::va() const {
    return va_;
}
inline uintptr_t vmiter::last_va() const {
    return (va_ | pageoffmask(level_)) + 1;
}
inline bool vmiter::low() const {
    return va_ <= VA_LOWMAX;
}
inline uint64_t vmiter::pa() const {
    if (*pep_ & PTE_P) {
        uintptr_t pa = *pep_ & PTE_PAMASK;
        if (level_ > 0) {
            pa &= ~0x1000UL;
        }
        return pa + (va_ & pageoffmask(level_));
    } else {
        return -1;
    }
}
template <typename T>
inline T vmiter::kptr() const {
    return reinterpret_cast<T>(pa());
}
inline uint64_t vmiter::perm() const {
    if (*pep_ & PTE_P) {
        return *pep_ & perm_;
    } else {
        return 0;
    }
}
inline bool vmiter::perm(uint64_t p) const {
    return (*pep_ & perm_ & p) == p;
}
inline bool vmiter::present() const {
    return (*pep_ & PTE_P) != 0;
}
inline bool vmiter::writable() const {
    return perm(PTE_P | PTE_W);
}
inline bool vmiter::user() const {
    return perm(PTE_P | PTE_U);
}
inline vmiter& vmiter::find(uintptr_t va) {
    real_find(va);
    return *this;
}
inline vmiter& vmiter::operator+=(intptr_t delta) {
    return find(va_ + delta);
}
inline vmiter& vmiter::operator-=(intptr_t delta) {
    return find(va_ - delta);
}
inline void vmiter::next_range() {
    real_find(last_va());
}
inline int vmiter::map(void* pa_ptr, int perm) {
    return map((uintptr_t) pa_ptr, perm);
}

inline ptiter::ptiter(x86_64_pagetable* pt, uintptr_t va)
    : pt_(pt) {
    go(va);
}
inline ptiter::ptiter(const proc* p, uintptr_t va)
    : ptiter(p->pagetable, va) {
}
inline uintptr_t ptiter::va() const {
    return va_ & ~pageoffmask(level_);
}
inline uintptr_t ptiter::last_va() const {
    return (va_ | pageoffmask(level_)) + 1;
}
inline bool ptiter::active() const {
    return va_ <= VA_NONCANONMAX;
}
inline bool ptiter::low() const {
    return va_ <= VA_LOWMAX;
}
inline int ptiter::level() const {
    return level_ - 1;
}
inline void ptiter::next() {
    down(true);
}
inline uintptr_t ptiter::ptp_pa() const {
    return *pep_ & PTE_PAMASK;
}
inline x86_64_pagetable* ptiter::ptp() const {
    return reinterpret_cast<x86_64_pagetable*>(ptp_pa());
}

inline bool vmiter::perm(uintptr_t last_va, uint64_t p) {
    while (va_ < last_va) {
        if (!perm(p)) {
            return false;
        }
        next_range();
    }
    return true;
}

template <typename T>
inline bool vmiter::low_perm(T* first, T* last, uint64_t p) {
    uintptr_t firstva = reinterpret_cast<uintptr_t>(first);
    uintptr_t lastva = reinterpret_cast<uintptr_t>(last);
    return firstva <= lastva
        && lastva <= VA_LOWEND
        && (firstva == lastva
            || find(firstva).perm(lastva, p));
}

template <typename T>
inline bool vmiter::low_perm(T* first, size_t size, uint64_t p) {
    uintptr_t firstva = reinterpret_cast<uintptr_t>(first);
    return firstva <= VA_LOWEND
        && size <= VA_LOWEND - firstva
        && (size == 0
            || find(firstva).perm(firstva + size, p));
}

#endif
