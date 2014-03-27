/**
 * @file client_primitive.hh
 */
#ifndef _CLIENT_PRIMITIVE_HH_
#define _CLIENT_PRIMITIVE_HH_

/**
 * Pure virtual interface for a client primitive.
 */
class ClientPrimitiveInterface {

public:

  /**
   * Default constructor.
   */
  ClientPrimitiveInterface() {}

  /**
   * Default destructor.
   */
  virtual ~ClientPrimitiveInterface() {}

};

/**
 * Dummy class for easier access and testing.
 */
class ClientPrimitive : public virtual ClientPrimitiveInterface {

public:

  /// @copydoc ClientPrimitiveInterface::ClientPrimitiveInterface()
  ClientPrimitive() {}
  /// @copydoc ClientPrimitiveInterface::~ClientPrimitiveInterface()
  virtual ~ClientPrimitive() {}

};

#endif
