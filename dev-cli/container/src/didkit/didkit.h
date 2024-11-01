#pragma once

// didkit.h - autogenerated by cbindgen

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * Issue a Verifiable Presentation for [DIDAuth](https://w3c-ccg.github.io/vp-request-spec/#did-authentication-request). Input parameters are the holder URI as a C string, and JSON C strings for the linked data proof options and the JWK for signing. On success,
 * a newly-issued verifiable presentation is returned as a newly-allocated C string. The
 * returned string should be freed using [`didkit_free_string`]. On failure, `NULL` is returned, and the
 * error message can be retrieved using [`didkit_error_message`].
 */
const char *didkit_did_auth(const char *holder,
                            const char *proof_options_json,
                            const char *key_json);

/**
 * Resolve a DID to a DID Document. Arguments are a C string containing the DID to resolve, and a
 * C string containing a JSON object for resolution input metadata. The return value on success is
 * a newly-allocated C string containing either the resolved DID document or a DID resolution
 * result JSON object. On error, `NULL` is returned, and the error can be retrieved using
 * [`didkit_error_message`].
 */
const char *didkit_did_resolve(const char *did, const char *input_metadata_json);

/**
 * Resolve a DID to a DID Document. Arguments are a C string containing the DID URL to dereference, and a
 * C string containing a JSON object for dereferencing input metadata. The return value on success is
 * a newly-allocated C string containing either a resolved resource or a DID resolution
 * result JSON object. On error, `NULL` is returned, and the error can be retrieved using
 * [`didkit_error_message`].
 */
const char *didkit_did_url_dereference(const char *did_url,
                                       const char *input_metadata_json);

/**
 * Retrieve a numeric code for the most recent error encountered by a DIDKit C function. If there
 * has not been an error, 0 is returned.
 */
int didkit_error_code(void);

/**
 * Retrieve a human-readable description of the most recent error encountered by a DIDKit C
 * function. The returned string is valid until the next call to a DIDKit function in the current
 * thread, and should not be mutated or freed. If there has not been any error, `NULL` is returned.
 */
const char *didkit_error_message(void);

/**
 * Free a C string that has been dynamically allocated by DIDKit. This should be used for strings
 * returned from most DIDKit C functions, per their respective documentation.
 */
void didkit_free_string(const char *string);

/**
 * Get the version of the DIDKit library. Returns a static C string which should not be mutated or
 * freed.
 */
const char *didkit_get_version(void);

/**
 * Convert a key in JWK format to a did:key DID. Input should be a JWK containing public key
 * parameters. Private key parameters in the JWK are ignored. On success, returns a
 * newly-allocated C string containing a DID corresponding to the JWK. The returned string must be
 * freed
 * with [`didkit_free_string`].  On failure, returns `NULL`; the error message can be retrieved
 * with [`didkit_error_message`].
 */
const char *didkit_key_to_did(const char *method_pattern, const char *jwk);

/**
 * Convert a key to a `did:key` DID URI for use in the `verificationMethod` property of a linked data
 * proof. Input should be a C string containing the key as a JWK. The JWK should contain public
 * key material; private key parameters are ignored. On success, this function returns a newly-allocated C string containing the `verificationMethod` URI. On failure, `NULL` is returned; the
 * error message can be retrieved using [`didkit_error_message`].
 */
const char *didkit_key_to_verification_method(const char *method_pattern,
                                              const char *jwk);

/**
 * Generate a new Ed25519 keypair in JWK format. On success, returns a pointer to a
 * newly-allocated string containing the JWK. The string must be freed with [`didkit_free_string`]. On
 * failure, returns `NULL`; the error message can be retrieved with [`didkit_error_message`].
 */
const char *didkit_vc_generate_ed25519_key(void);

/**
 * Issue a Verifiable Credential. Input parameters are JSON C strings for the unsigned credential
 * to be issued, the linked data proof options, and the JWK for signing.  On success, the
 * newly-issued verifiable credential is returned as a newly-allocated C string.  The returned
 * string should be freed using [`didkit_free_string`]. On failure, `NULL` is returned, and the error
 * message can be retrieved using [`didkit_error_message`].
 */
const char *didkit_vc_issue_credential(const char *credential_json,
                                       const char *proof_options_json,
                                       const char *key_json);

/**
 * Issue a Verifiable Presentation. Input parameters are JSON C strings for the unsigned
 * presentation to be issued, the linked data proof options, and the JWK for signing. On success,
 * the newly-issued verifiable presentation is returned as a newly-allocated C string. The
 * returned string should be freed using [`didkit_free_string`]. On failure, `NULL` is returned, and the
 * error message can be retrieved using [`didkit_error_message`].
 */
const char *didkit_vc_issue_presentation(const char *presentation_json,
                                         const char *proof_options_json,
                                         const char *key_json);

/**
 * Verify a Verifiable Credential. Arguments are a C string containing the Verifiable Credential
 * to verify, and a C string containing a JSON object for the linked data proof options for
 * verification. The return value is a newly-allocated C string containing a JSON object for the
 * verification result, or `NULL` in case of certain errors. On successful verification, the
 * verification result JSON object contains a "errors" property whose value is an empty array. If
 * verification fails, either `NULL` is returned and the error can be retrieved using
 * [`didkit_error_message`], or a verification result JSON object is returned with an "errors" array
 * containing information about the verification error(s) encountered. A string returned from this
 * function should be freed using [`didkit_free_string`].
 */
const char *didkit_vc_verify_credential(const char *credential, const char *proof_options_json);

/**
 * Verify a Verifiable Presentation. Arguments are a C string containing the Verifiable
 * Presentation to verify, and a C string containing a JSON object for the linked data proof
 * options for verification. The return value is a newly-allocated C string containing a JSON
 * object for the verification result, or `NULL` in case of certain errors. On successful
 * verification, the verification result JSON object contains a "errors" property whose value is
 * an empty array. If verification fails, either `NULL` is returned and the error can be retrieved
 * using [`didkit_error_message`], or a verification result JSON object is returned with an "errors"
 * array containing information about the verification error(s) encountered. A string returned
 * from this function should be freed using [`didkit_free_string`].
 */
const char *didkit_vc_verify_presentation(const char *presentation, const char *proof_options_json);