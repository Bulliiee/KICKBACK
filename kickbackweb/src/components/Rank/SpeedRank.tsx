import React, { useEffect, useState } from 'react'
import { RankBox, SearchBox, SearchInput, SearchBtn, MyBox, UserRankBox, RankTable, PageNation } from '../../styles/Rank/Speed'
import useUserStore from '../../stores/UserStore';
import { useShallow } from 'zustand/react/shallow';
import useAuthStore from '../../stores/AuthStore';
import axios from 'axios';

import { useNavigate } from 'react-router';
import MexicoTrack from "../../assets/MexicoTrack.png"
import CebuTrack from "../../assets/CebuTrack.png"

interface SearchData {
  nickname: string,
  profileImage: string,
  rank: number,
  time: string
}

const SpeedRank = () => {
  const [search, setSearch] = useState("");
  const [searchData, setSearchData] = useState<SearchData>();
  const [mapName,setMapName] = useState("MEXICO")
  const [userList, setUserList] = useState([]);
  const [isSearch, setIsSearch] = useState(true);
  const [saveUserList, setSaveUserList] = useState([]);
  const navigate = useNavigate();

  const { PATH, isLogin } =
    useAuthStore(
      useShallow((state) => ({
        PATH: state.PATH,
        isLogin: state.isLogin,
      }))
    );

  const { nickname } =
    useUserStore(
      useShallow((state) => ({
        nickname: state.nickname,
      }))
    );

  const changeSearch = (event: React.ChangeEvent<HTMLInputElement>) => {
    setSearch(event.target.value);
  };

  const searchUser = (event: React.FormEvent<HTMLFormElement>) => {
    event.preventDefault();

    if (search === "") {
      setIsSearch(!isSearch);
      axios.get(`${PATH}/api/v1/ranking/search2`, {
        params: {
          mapName,
          nickname,
        }
      })
        .then((res) => {
          setSearchData(res.data.dataBody)
        })
        .catch((error) => {
          console.log(error)
        })
    }

    axios.get(`${PATH}/api/v1/ranking/search2`, {
      params: {
        mapName,
        nickname: search,
      }
    })
      .then((res) => {
        setSearchData(res.data.dataBody)
      })
      .catch((error) => {
        console.log(error)
      })

    const searchUser = saveUserList.filter((item) => item["nickname"] === search);
    setUserList(searchUser);
    setCurrentPage(1);
  };

  const ITEMS_PER_PAGE = 10;
  const PAGINATION_NUMBERS = 5;

  const [currentPage, setCurrentPage] = useState(1);

  const totalPageCount = Math.ceil(userList.length / ITEMS_PER_PAGE);

  const handlePageChange = (page: number) => {
    setCurrentPage(page);
  };

  const renderPaginationNumbers = () => {
    const paginationNumbers = [];
    const start =
      Math.floor((currentPage - 1) / PAGINATION_NUMBERS) * PAGINATION_NUMBERS +
      1;

    for (
      let i = start;
      i < start + PAGINATION_NUMBERS && i <= totalPageCount;
      i++
    ) {
      paginationNumbers.push(
        <button
          key={i}
          onClick={() => handlePageChange(i)}
          className={currentPage === i ? "current" : ""}
        >
          {i}
        </button>
      );
    }

    return paginationNumbers;
  };

  const startIdx = (currentPage - 1) * ITEMS_PER_PAGE;
  const endIdx = startIdx + ITEMS_PER_PAGE;
  const currentItems = userList.slice(startIdx, endIdx);

  useEffect(() => {

    if (isLogin) {
      // 특정 유저 검색 (기본 멕시코 맵)
      axios.get(`${PATH}/api/v1/ranking/search2`, {
        params: {
          mapName,
          nickname,
        }
      })
        .then((res) => {
          setSearchData(res.data.dataBody)
        })
        .catch((error) => {
          console.log(error)
        })
    }

    // 맵별 모든 유저 기록 (기본 멕시코 맵)
    axios.get(`${PATH}/api/v1/ranking/speed`, {
      params: {
        mapNum: 0,
        page: 1
      }
    })
      .then((res) => {
        setUserList(res.data.content);
        setSaveUserList(res.data.content);
      })
      .catch((error) => console.log(error))
  }, []);

  return (
    <RankBox>
      <SearchBox onSubmit={searchUser}>
        <SearchInput
          placeholder="닉네임을 검색해보세요!"
          value={search}
          onChange={changeSearch}
          maxLength={6}
        ></SearchInput>
        <SearchBtn>검색</SearchBtn>
      </SearchBox>
      <MyBox>
        <div className='item'>
          {isLogin ? <>
            <div className='content'>
              <img src={searchData?.profileImage === null ?  "https://cdn.pixabay.com/photo/2015/10/05/22/37/blank-profile-picture-973460_1280.png" : searchData?.profileImage} alt="프로필" />
            </div>
            <div className='content'>
              <div>닉네임 : {searchData?.nickname}</div>
              <div>랭킹 : {searchData?.rank === null ? "기록이 없습니다." : searchData?.rank}</div>
              <div>기록 : {searchData?.time}</div>
            </div>
          </> : <div>
          </div>}
        </div>
        <div className='item'>
          <img src={MexicoTrack} alt="트랙 이미지" />
        </div>
      </MyBox>
      <UserRankBox>
        <RankTable>
          <div className="container2">
            <div className="item2">등수</div>
            <div className="item2">닉네임</div>
            <div className="item2">기록</div>
          </div>
          {currentItems.map((user, idx) => (
            <div key={idx} className="container">
              <div className="item">
                {user["rank"] === 1
                  ? "🥇"
                  : user["rank"] === 2
                  ? "🥈"
                  : user["rank"] === 3
                  ? "🥉"
                  : user["rank"]}
              </div>
              <div className="item">
                <img
                  src={
                    user["profileImage"] === null
                      ? "https://cdn.pixabay.com/photo/2015/10/05/22/37/blank-profile-picture-973460_1280.png"
                      : user["profileImage"]
                  }
                  alt="프로필이미지"
                  style={{
                    width: "35px",
                    height: "35px",
                    borderRadius: "100px",
                    marginRight: "20px"
                  }}
                />
              </div>
              <div className="item">{user["nickname"]}</div>
              <div className="item">{user["time"]}</div>
            </div>
          ))}
          {userList.length === 0 ? (
            <div className="no_result">존재하지 않는 유저입니다.</div>
          ) : null}
        </RankTable>
        {totalPageCount > 0 && (
          <PageNation>
            <div className="nav_buttons">
              <button onClick={() => handlePageChange(1)}>&lt;&lt;</button>
              <button
                onClick={() => handlePageChange(currentPage - 1)}
                disabled={currentPage === 1}
              >
                &lt;
              </button>
              {renderPaginationNumbers()}
              <button
                onClick={() => handlePageChange(currentPage + 1)}
                disabled={currentPage === totalPageCount}
              >
                &gt;
              </button>
              <button onClick={() => handlePageChange(totalPageCount)}>
                &gt;&gt;
              </button>
            </div>
          </PageNation>
        )}
      </UserRankBox>
    </RankBox>
  )
}

export default SpeedRank