local didkit = require("didkit")

local credential = [[{
    "@context": ["https://www.w3.org/2018/credentials/v1"],
    "id": "http://example.edu/credentials/3732",
    "type": ["VerifiableCredential"],
    "issuer": "https://example.com/issuers/14",
    "credentialSubject": {
        "id": "did:example:ebfeb1f712ebc6f1c276e12ec21"
    }
}]]

local options = "{}"

didkit.verify_credential(credential, options)