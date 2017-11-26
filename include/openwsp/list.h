/*
 *  OpenWSP (an opensource webstream codec)
 *  Copyleft (C) 2016, The 1st Middle School in Yongsheng Lijiang China
 *  please contact with <diyer175@hotmail.com> if you have any problems.
 *
 *  This project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License(GPL)
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#ifndef OPENWSP_LIST_H_
#define OPENWSP_LIST_H_

#include <new>
#include <typeinfo>

#include <openwsp/err.h>
#include <openwsp/assert.h>

namespace openwsp {

#define WSLIST_RTTI 0

/***************************************************
  *****       Generic List object              *****
  ***************************************************/

template <class listT>
class WSList {
public:
    WSList() {
        root =
        cur =
        prev =
        next = 0;
        initer = false;
    }
    ~WSList() {
        RemoveAll();
    }

    /**
     * Push a item to the front of list
     */
    int Pushfront(listT data) {
        int rc = WERR_FAILED;
        WSList<listT> *nnode;
        WSList<listT> *cnode;
        /*
         * Any trying to insert a node when we're in
         * iterating mode is not allowed.
         */
        if (initer)
            return WERR_ITERATING;
        /*
         * We should avoid this memory allocating being done
         * in class construction function.
         */
        if (!root) {
            rc = allocNode(&root);
            nnode = 0;
            cnode = root;
        } else {
            rc = allocNode(&root->prev);
            nnode = root;
            cnode = root->prev;
        }

        if (WS_SUCCESS(rc)) {
            cnode->data = data;
            cnode->prev = 0;
            cnode->next = nnode;
            root = cnode;
            return WINF_SUCCEEDED;
        }
        return rc;
    }

    /**
     * Begin the iterating
     */
    void begin() {
        cur = root;
    }

    /**
     * End the iterating.
     * @return true. next element is valid.
     * @return false. reached at the end.
     */
    bool end() {
        return (cur) ? true : false;
    }

    /**
     * Get the name of current list node.
     * @param out           Where to store the filename. Be careful that
     *                      it's only a pointer to the data of list.When the
     *                      List is free, this pointer will become invalid.
     * @return status code
     */
    int get(listT &out) {
        if (cur) {
            out = cur->data;
            return WINF_SUCCEEDED;
        }
        return WERR_FAILED;
    }

    /**
     * Go to the next element in the list.
     * @return status code
     */
    int tonext() {
        if (cur) {
            cur = cur->next;
            return WINF_SUCCEEDED;
        }
        return WERR_FAILED;
    }

    WSList<listT>& operator++(int) {
        tonext();
        return *this;
    }

    /**
     * Go to the previous element in the list.
     * @return status code
     */
    int toprev() {
        if (cur) {
            cur = cur->prev;
            return WINF_SUCCEEDED;
        }
        return WERR_FAILED;
    }

    WSList<listT>& operator--(int) {
        toprev();
        return *this;
    }

    /**
     * Tell whether the next element is valid.
     * @return true if valid
     * @return false if invalid
     */
    bool NextValid() {
        return (cur && cur->next);
    }

    /**
     * Tell whether the previous element is valid.
     * @return true if valid
     * @return false if invalid
     */
    bool PrevValid() {
        return (cur && cur->prev);
    }

    /**
     * Remove all the elements.
     */
    int RemoveAll() {
        WSList<listT> *pnode;
        WSList<listT> *node = root;
        while(node) {
            pnode = node;
            node = node->next;
            delete pnode;
        };
        root = 0;
        cur = 0;
        return WINF_SUCCEEDED;
    }

    /**
     * Remove all the elements and Delete the memory of elements.
     * @return WINF_SUCCEEDED if succeeded.
     * @return WERR_TYPE_MISMATCH if the elements are not in pointer type.
     */
    int RemoveAllPtr() {
        int rc = WINF_SUCCEEDED;
        WSList<listT> *pnode;
        WSList<listT> *node = root;
        while(node) {
            pnode = node;
            node = node->next;
            // type depended deleting
#if LIST_RTTI
            if ( (typeid(listT) == typeid(char*)) ||
                 (typeid(listT) == typeid(int*)) )
                 /* add more there */ {
#endif
                delete pnode->data;
#if LIST_RTTI
            } else {
                rc = WERR_TYPE_MISMATCH;
            }
#else
            rc = WINF_SUCCEEDED;
#endif
            delete pnode;
        };
        root = 0;
        cur = 0;
        return rc;
    }

protected:
    /**
     * Allocate a new list
     * @param node      Where to store the node.
     * @return status code
     */
    int allocNode(WSList<listT> **node) {
        WS_ASSERT(node);
        /*
         create a new node in heap
         */
        *node = new (std::nothrow) WSList<listT>;
        if (!*node)
            return WERR_ALLOC_MEMORY;

        /*
         re-initiate the node
         */
        (*node)->prev = 0;
        (*node)->next = 0;
        return WINF_SUCCEEDED;
    }

    /**
     * Release the enum list.
     * @return status code
     */
    void freeNode(WSList<listT> *node) {
        WS_ASSERT(node);
        delete node;
    }

private:
    WSList<listT> *root;
    WSList<listT> *cur;
    WSList<listT> *next;
    WSList<listT> *prev;
    listT   data;
    bool    initer;
};

} // namespace openwsp

#endif //!defined(OPENWSP_LIST_H_)
