//
//  Copyright (c) 2019 Blackberry Inc. All rights reserved.
//

#pragma once

#ifdef __cplusplus
#include <memory>


#ifndef GD_C_API
#   if !defined(_WIN32)
#       define GD_C_API __attribute__((visibility("default")))
#   else
#       define GD_C_API
#   endif
#endif

namespace GD  {
    class SafeUrl

    /**
    Check if URLs are safe using CylancePROTECT (C++).

    CylancePROTECT uses a combination of cloud and on-device based AI and machine learning to identify malware and unsafe URLs. It
    is integrated into the Dynamics HTTP secure communication APIs and when licensed provides protection without further
    integration being required.

    If an application wants to check a URL outside of utilizing GDURLLoadingSystem   this
    class can be used to first confirm if CylancePROECT is enabled for the user (SafeUrl::isSafeBrowsingEnabled)
    and then to check if specific URLs are safe (SafeUrl::checkUrl)

    Caller class needs to implement the callback class and wrap it into shared pointer and pass it as a weak pointer.
    The reference number is optional. These APIs can be called only after SDK has been initialised (i.e. after the authorized event).
     
    The caller can use this API in two modes:
     
    1. One-to-one mode: in this case, you need to have separate callback object (wrapped in a shared pointer) for each URL request.
    However, this will not scale well.
    2. Dispatch mode: you can keep a single callback object and use a reference number for matching your URLs and results. This will scale well.
     
    Please note that SafeUrl class follows the "Monostate" design pattern, that is - you can create as many instances
    of this class as you wish and all will behave as if they are the same object.
    */
    {
    public:
        
        /**
        * Result codes from testing if a URL is safe.
        *
        * These constants can determine the result returned by CylancePROTECT safe browsing detection.
        */
        enum class UrlCheckResult {
            /** The result could not be determined.
             */
            UNKNOWN         = 0,
            /** The URL is deemed to be safe.
             */
            SAFE            = 1,
            /** The URL is flagged as being unsafe or malicious and should not be called.
             */
            UNSAFE          = 20
        };
        

        /** Error codes from testing if a URL is safe.
        *
        * These constants can be used for handling errors returned by CylancePROTECT safe browsing detection.
        */
        enum class UrlCheckError {
            /** No error has been detected.
             */
            NONE            = 0,
            /** There was a networking error whilst trying to check if the URL was safe.
             */
            NETWORK         = 1,
            /** Another error was encountered whilst checking if the URL was safe.
             */
            GENERAL         = 2
        };

        /**
         * Item within the SafeURL callback.
         *
         * Item which includes the results, errors and optional reference retured by SafeURL callback.
         */
        
        class SafeUrlResultItem {

        public:
            
            SafeUrlResultItem(const UrlCheckResult& result, const UrlCheckError& error, const long long reference );
            
        public:
            UrlCheckResult result;
            UrlCheckError error;
            long long reference;
        };
        
        /**
         * Callback object returned from SafeURL
         *
         * Object includes an item with the results and error status from SafeURL
         */

        class SafeUrlCallback {

        public:
            virtual void completed( const SafeUrlResultItem& resultItem ) = 0;
            virtual ~SafeUrlCallback() = default;
        };

        
        /**
         * This function checks whether or not the url is safe to make an http request.
         * The result is returned to the callback object SafeUrlCallback that is passed by the caller.
         *
         * @param url <tt>string</tt> A string containing the URL.\n
         *
         * @param callback <tt>std::weak_ptr<SafeUrlCallback></tt> A callback object that is called when the result is obtained .\n
         *
         * @param reference <tt>long long</tt> An optional parameter that is set by the caller, which is returned to the callback object. \n
         *
         * @param suppressUserPrompt <tt>boolean</tt> An optional boolean parameter to suppress the user
         interface prompt shown by the Dynamics Runtime if the URL is unsafe. \n
         *
         */
        GD_C_API void checkUrl(const std::string  &url, const std::weak_ptr<SafeUrlCallback> callback,
                      const long long reference = 0, const bool suppressUserPrompt = false) const;
        
        
        
        /**
         * Determine if Safe Browsing is enabled.
         *
         * This function returns whether or not the safe browsing feature within CylancePROTECT is enabled for the user.
         *
         * @return <tt>bool</tt> true if enabled, false if disabled.\n
         */
        GD_C_API bool isSafeBrowsingEnabled();
    };
    
};
#endif
