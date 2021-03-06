#ifndef VISITABLE_H
#define VISITABLE_H

#include <vector>
#include <list>
#include <functional>

#include "enums.h"

class item;

enum class VisitResponse {
    ABORT, // Stop processing after this node
    NEXT,  // Descend vertically to any child nodes and then horizontally to next sibling
    SKIP   // Skip any child nodes and move directly to the next sibling
};

template <typename T>
class visitable
{
    public:
        /**
         * Traverses this object and any child items contained using a visitor pattern
         *
         * @param func visitor function called for each node which controls whether traversal continues.
         * The first argument is the node and the second is the parent node (if any)
         *
         * The visitor function should return VisitResponse::Next to recursively process child items,
         * VisitResponse::Skip to ignore children of the current node or VisitResponse::Abort to skip all remaining nodes
         *
         * @return This method itself only ever returns VisitResponse::Next or VisitResponse::Abort.
         */
        VisitResponse visit_items_with_parent(
            const std::function<VisitResponse( item *, item * )> &func );
        VisitResponse visit_items_with_parent_const(
            const std::function<VisitResponse( const item *, const item * )> &func ) const;

        /** Lightweight version which provides only the current node */
        VisitResponse visit_items( const std::function<VisitResponse( item * )> &func );
        VisitResponse visit_items_const( const std::function<VisitResponse( const item * )> &func ) const;

        /**
         * Determine the immediate parent container (if any) for an item.
         * @param it item to search for which must be contained (at any depth) by this object
         * @return parent container or nullptr if the item is not within a container
         */
        item *find_parent( const item &it );
        const item *find_parent( const item &it ) const;

        /**
         * Returns vector of parent containers (if any) starting with the innermost
         * @param it item to search for which must be contained (at any depth) by this object
         */
        std::vector<item *> parents( const item &it );
        std::vector<const item *> parents( const item &it ) const;

        /** Returns true if this visitable instance contains the item */
        bool has_item( const item &it ) const;

        /** Returns true if any item (including those within a container) matches the filter */
        bool has_item_with( const std::function<bool( const item & )> &filter ) const;

        /**
         * Removes items contained by this instance which match the filter
         * @note if this instance itself is an item it will not be considered by the filter
         * @param filter a UnaryPredicate which should return true if the item is to be removed
         * @param count maximum number of items to if unspecified unlimited. A count of zero is a no-op
         * @return any items removed (items counted by charges are not guaranteed to be stacked)
         */
        std::list<item> remove_items_with( const std::function<bool( const item & )> &filter,
                                           int count = INT_MAX );

        /** Removes and returns the item which must be contained by this instance */
        item remove_item( item &it );
};

#endif
