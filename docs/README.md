# syspilot

Shared client-side library for reusable code needed by the main application,
elevator, parser, and client environment management.

## Module properties

Module: `syspilot`
Dir type: `subproject`
Bin type: `library`
Target: `uniter_syspilot`
Alias: `uniter::syspilot`
Hard dependencies: none
Soft dependencies: `---`
Version: `0.0.2`
MVP: `+`

Related stack documentation: [Project stack](../../../docs/project-stack.md).

## Runtime directories

`PathResolver` places runtime data exclusively below Qt's
`QStandardPaths::TempLocation`:

```text
<temp>/database
<temp>/filecache
<temp>/localminio
<temp>/updates
```

There is no fallback location. If Qt does not provide a temporary directory,
these paths remain unresolved and `ensure_exists` fails. Explicit `DirType`
overrides remain available for tests and controlled deployments.

## Windows credential storage

`<secure/secure.h>` exposes `syspilot::CredentialStorage`, a keyed wrapper over
Windows Credential Manager. A record contains an optional `QString` username
and an opaque `QByteArray` secret. Writes persist for the current Windows user
on the local machine and replace an existing record with the same target.

The caller owns target construction. Targets must be stable and
application-qualified; company lookup and company-to-target mapping remain in
the registry/settings layer. The wrapper does not enumerate accounts or define
whether a secret is a password, refresh token, or other private material.
The wrapper clears the native buffer returned by Windows, but returned
`QByteArray` instances and their copies remain caller-owned sensitive memory.

```cpp
syspilot::CredentialStorage storage;
const QString target = makeCredentialTarget(companyFromSettings);

const auto result = storage.read(target);
if(result.ok()) {
    useCredential(result.credential->username, result.credential->secret);
}
```

On non-Windows platforms the same API is available but returns
`CredentialStatus::UnsupportedPlatform`.
