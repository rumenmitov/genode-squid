/**
 * @Author Rumen Mitov
 * @Date 2024-09-07

 squid.h provides an API to snapshot data to the disk.
 It is organized as a Radix Tree with an Upper Directory, Middle Directory, and a
 Lower Directory.

 Upper      - determined by the first two chars of the hash.
 Middle     - determined by the next pair of chars of the hash.
 Lower      - determined by the third pair of chars of the hash.
 Squid File - determined by remaining chars in hash.
*/

#ifndef __SQUID_H
#define __SQUID_H

#ifdef __cplusplus

#include <base/component.h>
#include <base/sleep.h>
#include <base/attached_rom_dataspace.h>
#include <base/heap.h>
#include <os/vfs.h>
#include <vfs/file_system_factory.h>
#include <base/buffered_output.h>

namespace Squid_snapshot {
    using namespace Genode;

    /**
     * @brief Error types associated with Squid Cache.
     */
    enum Error {
	WriteFile,
	ReadFile,
	CreateFile,
	CorruptedFile,
	None
    };

    struct Main
    {
	Env &_env;
	Main(Env &env) : _env(env) {}

	Heap _heap { _env.ram(), _env.rm() };


	Attached_rom_dataspace _config { _env, "config" };

	Vfs::Global_file_system_factory _fs_factory { _heap };

	Vfs::Simple_env _vfs_env { _env, _heap, _config.xml().sub_node("vfs") };

	typedef Directory::Path Path;

	Directory _root_dir { _vfs_env };


	const int HASH_LEN = 32;

	/**
         * @brief Generates a hash based on timestamp. #warning NEEDS TO BE UPDATED!!!
         */
	void _hash(char *me);

	/**
         * @brief Writes payload to file (creates one if it does not exist).
         */	
	enum Error _write(char const *path, void *payload, size_t size);

	
       /**
        * @brief Reads from squid file into payload buffer.
	*/
	enum Error _read(char const *path, void *payload);

	/**
         * @brief Deletes squid file from filesystem.
	 */
	enum Error _delete(char const *path);


	/**
	 * @brief Unit test.
	 */
	enum Error _test(void);

    };

    extern Main *global_squid;
};


#endif // __cplusplus

#endif // __SQUID_H
