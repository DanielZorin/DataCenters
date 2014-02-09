/*
 * resourceType.h
 *
 *  Created on: Feb 9, 2014
 *      Author: paul
 */

#ifndef RESOURCETYPE_H_
#define RESOURCETYPE_H_

#pragma once

class ResourceType {

// Enumerations
public:
    /*
     * Restrictions of resource. Restriction specifies the way resources are calculated
     * and checked
     */
    enum ResourceRestriction {
        NONE = 0,
        // The sum of virtual resources to allocate should be less or equal to physical resources
        ADDITIVE = 1,
        // Virtual resource should be less then or equal to physical resource to allocate.
        SHOULD_BE_LESS_OR_EQUAL = 2,
        // Virtual resource should be greater then or equal to physical resource to allocate.
        SHOULD_BE_GREATER_OR_EQUAL = 3,
        // Virtual resource should be equal to physical resource to allocate.
        SHOULD_BE_EQUAL = 4
    };

// Constructor
public:

    ResourceType(const ResourceRestriction resourceRestriction)
    : resourceRestriction(resourceRestriction) {
    }

// Useful methods
public:

    /*
     * Check whether assignment of specified resources is possible
     */
    inline bool isAssignmentPossible(unsigned virtualResource, unsigned availablePhysicalResource) {
        if ( (int)resourceRestriction < 3 )
            return virtualResource <= availablePhysicalResource;

        if ( resourceRestriction == SHOULD_BE_GREATER_OR_EQUAL )
            return virtualResource >= availablePhysicalResource;

        return virtualResource == availablePhysicalResource;
    }

    /*
     * Check whether the resource is countable, i.e. we can decrease/increase it's value
     */
    inline bool isCountable() {
        return resourceRestriction == ADDITIVE;
    }

private:
    ResourceRestriction resourceRestriction;
};


#endif /* RESOURCETYPE_H_ */
