/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <glib-object.h>
#include "config.h"

#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>
#include "DOMObjectCache.h"
#include "ExceptionCode.h"
#include "GObjectEventListener.h"
#include "JSMainThreadExecState.h"
#include "Node.h"
#include "TestEventTarget.h"
#include "WebKitDOMBinding.h"
#include "WebKitDOMEventPrivate.h"
#include "WebKitDOMEventTarget.h"
#include "gobject/ConvertToUTF8String.h"
#include "webkit/WebKitDOMEvent.h"
#include "webkit/WebKitDOMEventPrivate.h"
#include "webkit/WebKitDOMNode.h"
#include "webkit/WebKitDOMNodePrivate.h"
#include "webkit/WebKitDOMTestEventTarget.h"
#include "webkit/WebKitDOMTestEventTargetPrivate.h"
#include "webkitdefines.h"
#include "webkitglobalsprivate.h"
#include "webkitmarshal.h"

namespace WebKit {
    
WebKitDOMTestEventTarget* kit(WebCore::TestEventTarget* obj)
{
    g_return_val_if_fail(obj, 0);

    if (gpointer ret = DOMObjectCache::get(obj))
        return static_cast<WebKitDOMTestEventTarget*>(ret);

    return static_cast<WebKitDOMTestEventTarget*>(DOMObjectCache::put(obj, WebKit::wrapTestEventTarget(obj)));
}
    
} // namespace WebKit //

static void webkit_dom_test_event_target_dispatch_event(WebKitDOMEventTarget* target, WebKitDOMEvent* event, GError** error)
{
    WebCore::Event* coreEvent = WebKit::core(event);
    WebCore::TestEventTarget* coreTarget = static_cast<WebCore::TestEventTarget*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    WebCore::ExceptionCode ec = 0;
    coreTarget->dispatchEvent(coreEvent, ec);
    if (ec) {
        WebCore::ExceptionCodeDescription description(ec);
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.code, description.name);
    }
}

static gboolean webkit_dom_test_event_target_add_event_listener(WebKitDOMEventTarget* target, const char* eventName, GCallback handler, gboolean bubble, gpointer userData)
{
    WebCore::TestEventTarget* coreTarget = static_cast<WebCore::TestEventTarget*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return WebCore::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, bubble, userData);
}

static gboolean webkit_dom_test_event_target_remove_event_listener(WebKitDOMEventTarget* target, const char* eventName, GCallback handler, gboolean bubble)
{
    WebCore::TestEventTarget* coreTarget = static_cast<WebCore::TestEventTarget*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return WebCore::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, bubble);
}

static void webkit_dom_event_target_init(WebKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_test_event_target_dispatch_event;
    iface->add_event_listener = webkit_dom_test_event_target_add_event_listener;
    iface->remove_event_listener = webkit_dom_test_event_target_remove_event_listener;
}

WebKitDOMNode*
webkit_dom_test_event_target_item(WebKitDOMTestEventTarget* self, gulong index)
{
    g_return_val_if_fail(self, 0);
    WebCore::JSMainThreadNullState state;
    WebCore::TestEventTarget * item = WebKit::core(self);
    PassRefPtr<WebCore::Node> g_res = WTF::getPtr(item->item(index));
    WebKitDOMNode* res = WebKit::kit(g_res.get());
    return res;
}

gboolean
webkit_dom_test_event_target_dispatch_event(WebKitDOMTestEventTarget* self, WebKitDOMEvent* evt, GError **error)
{
    g_return_val_if_fail(self, 0);
    WebCore::JSMainThreadNullState state;
    WebCore::TestEventTarget * item = WebKit::core(self);
    g_return_val_if_fail(evt, 0);
    WebCore::Event * converted_evt = NULL;
    if (evt != NULL) {
        converted_evt = WebKit::core(evt);
        g_return_val_if_fail(converted_evt, 0);
    }
    WebCore::ExceptionCode ec = 0;
    gboolean res = item->dispatchEvent(converted_evt, ec);
    if (ec) {
        WebCore::ExceptionCodeDescription ecdesc(ec);
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), ecdesc.code, ecdesc.name);
    }
    return res;
}


G_DEFINE_TYPE_WITH_CODE(WebKitDOMTestEventTarget, webkit_dom_test_event_target, WEBKIT_TYPE_DOM_OBJECT, G_IMPLEMENT_INTERFACE(WEBKIT_TYPE_DOM_EVENT_TARGET, webkit_dom_event_target_init))

namespace WebKit {

WebCore::TestEventTarget* core(WebKitDOMTestEventTarget* request)
{
    g_return_val_if_fail(request, 0);

    WebCore::TestEventTarget* coreObject = static_cast<WebCore::TestEventTarget*>(WEBKIT_DOM_OBJECT(request)->coreObject);
    g_return_val_if_fail(coreObject, 0);

    return coreObject;
}

} // namespace WebKit
enum {
    PROP_0,
};


static void webkit_dom_test_event_target_finalize(GObject* object)
{
    WebKitDOMObject* dom_object = WEBKIT_DOM_OBJECT(object);
    
    if (dom_object->coreObject) {
        WebCore::TestEventTarget* coreObject = static_cast<WebCore::TestEventTarget *>(dom_object->coreObject);

        WebKit::DOMObjectCache::forget(coreObject);
        coreObject->deref();

        dom_object->coreObject = NULL;
    }

    G_OBJECT_CLASS(webkit_dom_test_event_target_parent_class)->finalize(object);
}

static void webkit_dom_test_event_target_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec)
{
    WebCore::JSMainThreadNullState state;
    switch (prop_id) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}


static void webkit_dom_test_event_target_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec)
{
    WebCore::JSMainThreadNullState state;
    switch (prop_id) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}


static void webkit_dom_test_event_target_constructed(GObject* object)
{

    if (G_OBJECT_CLASS(webkit_dom_test_event_target_parent_class)->constructed)
        G_OBJECT_CLASS(webkit_dom_test_event_target_parent_class)->constructed(object);
}

static void webkit_dom_test_event_target_class_init(WebKitDOMTestEventTargetClass* requestClass)
{
    GObjectClass *gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->finalize = webkit_dom_test_event_target_finalize;
    gobjectClass->set_property = webkit_dom_test_event_target_set_property;
    gobjectClass->get_property = webkit_dom_test_event_target_get_property;
    gobjectClass->constructed = webkit_dom_test_event_target_constructed;



}

static void webkit_dom_test_event_target_init(WebKitDOMTestEventTarget* request)
{
}

namespace WebKit {
WebKitDOMTestEventTarget* wrapTestEventTarget(WebCore::TestEventTarget* coreObject)
{
    g_return_val_if_fail(coreObject, 0);

    /* We call ref() rather than using a C++ smart pointer because we can't store a C++ object
     * in a C-allocated GObject structure.  See the finalize() code for the
     * matching deref().
     */
    coreObject->ref();

    return  WEBKIT_DOM_TEST_EVENT_TARGET(g_object_new(WEBKIT_TYPE_DOM_TEST_EVENT_TARGET,
                                               "core-object", coreObject, NULL));
}
} // namespace WebKit
